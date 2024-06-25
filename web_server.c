#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

#define PORT 8080
#define THREAD_POOL_SIZE 4
#define BUFFER_SIZE 1024
#define RESOURCE_DIR "./"

// Estructura para los argumentos del hilo
typedef struct {
    int socket;
} task_t;

// Cola para almacenar tareas
task_t task_queue[256];
int queue_size = 0;
int queue_capacity = 256;
int front = 0;
int rear = -1;

// Mutex y variables de condición para sincronizar la cola
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Función para añadir tareas a la cola
void enqueue(int client_socket) {
    pthread_mutex_lock(&lock);
    if (queue_size < queue_capacity) {
        rear = (rear + 1) % queue_capacity;
        task_queue[rear].socket = client_socket;
        queue_size++;
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&lock);
}

// Función para retirar una tarea de la cola
task_t dequeue() {
    pthread_mutex_lock(&lock);
    while (queue_size == 0) {
        pthread_cond_wait(&cond, &lock);
    }
    task_t task = task_queue[front];
    front = (front + 1) % queue_capacity;
    queue_size--;
    pthread_mutex_unlock(&lock);
    return task;
}

// Función para extraer el método y la ruta de la solicitud HTTP
void parse_http_request(const char* request, char* method, char* path) {
    sscanf(request, "%s %s", method, path);
}

// Función para enviar respuestas HTTP con contenido de archivo
void send_http_response(int client_fd, const char* path, const char* method, const char* body, size_t body_length) {
    char full_path[512];
    sprintf(full_path, "%s%s", RESOURCE_DIR, path);

    if (strcmp(method, "POST") == 0 || strcmp(method, "PUT") == 0) {
        // Abre o crea el archivo para escribir o actualizar
        int file_fd = open(full_path, O_WRONLY | O_CREAT | (strcmp(method, "POST") == 0 ? O_TRUNC : O_APPEND), 0666);
        if (file_fd == -1) {
            char* response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
            send(client_fd, response, strlen(response), 0);
        } else {
            write(file_fd, body, body_length);
            close(file_fd);
            char* response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
            send(client_fd, response, strlen(response), 0);
        }
    } else {
        int file_fd = open(full_path, O_RDONLY);
        struct stat file_stat;
        if (file_fd == -1 || fstat(file_fd, &file_stat) == -1) {
            char* header = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
            send(client_fd, header, strlen(header), 0);
        } else {
            char header[1024];
            sprintf(header, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\n\r\n", file_stat.st_size);
            send(client_fd, header, strlen(header), 0);

            if (strcmp(method, "HEAD") != 0) { // No enviar el cuerpo para HEAD
                sendfile(client_fd, file_fd, NULL, file_stat.st_size);
            }
            close(file_fd);
        }
    }
}

// Función que ejecutan los hilos de la alberca
void *handle_request(void *arg) {
    while (1) {
        task_t task = dequeue();
        int client_socket = task.socket;

        char buffer[BUFFER_SIZE * 4]; // Aumentar tamaño para permitir cuerpo más grande
        int read_bytes = read(client_socket, buffer, sizeof(buffer) - 1);
        buffer[read_bytes] = '\0';

        char method[10], path[255];
        parse_http_request(buffer, method, path);

        // Buscar el inicio del cuerpo en la solicitud (POST y PUT)
        char* body = strstr(buffer, "\r\n\r\n") + 4;
        size_t body_length = read_bytes - (body - buffer);

        if (strcmp(method, "GET") == 0 || strcmp(method, "HEAD") == 0 || strcmp(method, "POST") == 0 || strcmp(method, "PUT") == 0) {
            send_http_response(client_socket, path, method, body, body_length);
        } else {
            char* response = "HTTP/1.1 501 Not Implemented\r\nContent-Length: 0\r\n\r\n";
            send(client_socket, response, strlen(response), 0);
        }

        close(client_socket);  // Asegúrate de cerrar el socket una vez procesado
    }
    return NULL;
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Crear e iniciar los hilos de la alberca
    pthread_t threads[THREAD_POOL_SIZE];
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_create(&threads[i], NULL, handle_request, NULL);
    }

    // Configuración inicial del servidor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 10);

    printf("Servidor listo en puerto %d\n", PORT);

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        enqueue(client_socket);
    }

    return 0;
}

