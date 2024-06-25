#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define THREAD_POOL_SIZE 4

// Estructura para los argumentos del hilo
typedef struct {
    int socket;
} thread_args;

// Función para manejar las peticiones HTTP
void* handle_request(void* args) {
    thread_args* t_args = (thread_args*) args;
    int new_socket = t_args->socket;
    free(t_args);

    char buffer[30000] = {0};
    read(new_socket, buffer, 30000);
    printf("%s\n", buffer);

    // Aquí se podría implementar el parseo de las peticiones GET, POST, PUT

    char *response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    write(new_socket, response, strlen(response));
    close(new_socket);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t threads[THREAD_POOL_SIZE];

    // Crear el descriptor de archivo del socket del servidor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket creation");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Vincular el socket al puerto 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    // Poner el servidor a escuchar
    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        thread_args *t_args = malloc(sizeof(thread_args));
        t_args->socket = new_socket;

        pthread_create(&threads[new_socket % THREAD_POOL_SIZE], NULL, handle_request, (void*) t_args);
    }

    return 0;
}

