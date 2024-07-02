# Servidor Web en C

Este es un servidor web básico escrito en C que utiliza una alberca de hilos para manejar múltiples conexiones HTTP. El servidor puede manejar solicitudes GET, POST, PUT y HEAD para archivos estáticos y para la creación o actualización de archivos en el servidor.

## Estructura del Directorio

El servidor asume la siguiente estructura de directorios:

```
/
|- web_server.c      // El código fuente del servidor
|- resources/        // Directorio para los recursos servidos
   |- archivo_fuente
   |- archivo_prueba1
```

## Compilación

Para compilar el servidor, necesitarás tener instalado GCC y `pthread`. Puedes compilar el servidor utilizando el siguiente comando:

```bash
gcc -o web_server web_server.c -pthread
```

Este comando compila el código fuente en el archivo `web_server.c` y crea un ejecutable llamado `web_server`, enlazando la biblioteca `pthread` necesaria para la gestión de hilos.

## Ejecución

Para ejecutar el servidor en tu sistema, utiliza el siguiente comando:

```bash
./web_server
```# Servidor Web en C

Este es un servidor web básico escrito en C que utiliza una alberca de hilos para manejar múltiples conexiones HTTP. El servidor puede manejar solicitudes GET, POST, PUT y HEAD para archivos estáticos y para la creación o actualización de archivos en el servidor.

## Estructura del Directorio

El servidor asume la siguiente estructura de directorios:

```
/
|- web_server.c      // El código fuente del servidor
|- resources/        // Directorio para los recursos servidos
   |- archivo_fuente
      |- archivo_prueba1
      ```

## Compilación

Para compilar el servidor, necesitarás tener instalado GCC y `pthread`. Puedes compilar el servidor utilizando el siguiente comando:

```bash
gcc -o web_server web_server.c -pthread
```

Este comando compila el código fuente en el archivo `web_server.c` y crea un ejecutable llamado `web_server`, enlazando la biblioteca `pthread` necesaria para la gestión de hilos.

## Ejecución

Para ejecutar el servidor en tu sistema, utiliza el siguiente comando:

```bash
./web_server
```

Esto iniciará el servidor en el puerto 8080 y comenzará a escuchar conexiones entrantes. Asegúrate de que el puerto 8080 esté libre o modifica el puerto en el código fuente si es necesario.

## Pruebas

Para probar el servidor, puedes utilizar un navegador web o una herramienta de línea de comandos como `curl`. Aquí algunos ejemplos de cómo realizar solicitudes al servidor:

### Solicitar un archivo estático

```bash
curl http://localhost:8080/archivo_fuente
```

### Crear o actualizar un archivo

Para crear un nuevo archivo o actualizar un archivo existente utilizando `POST` o `PUT`:

```bash
curl -X POST -d "datos del cuerpo" http://localhost:8080/archivo_prueba1
curl -X PUT -d "datos actualizados" http://localhost:8080/archivo_prueba1
```

### Solicitar un archivo con método HEAD

El método `HEAD` se puede usar para obtener los metadatos de un archivo sin descargar el archivo en sí:

```bash
curl -I http://localhost:8080/archivo_fuente
```

## Notas Adicionales

- El servidor actualmente no implementa todas las características de un servidor web completo y está destinado principalmente para fines educativos y de prueba.
- - Asegúrate de que el directorio `resources` contiene los archivos necesarios para las pruebas y que los permisos de archivos son adecuados para que el servidor pueda leer y escribir en ellos.

Esto iniciará el servidor en el puerto 8080 y comenzará a escuchar conexiones entrantes. Asegúrate de que el puerto 8080 esté libre o modifica el puerto en el código fuente si es necesario.

## Pruebas

Para probar el servidor, puedes utilizar un navegador web o una herramienta de línea de comandos como `curl`. Aquí algunos ejemplos de cómo realizar solicitudes al servidor:

### Solicitar un archivo estático

```bash
curl http://localhost:8080/archivo_fuente
```

### Crear o actualizar un archivo

Para crear un nuevo archivo o actualizar un archivo existente utilizando `POST` o `PUT`:

```bash
curl -X POST -d "datos del cuerpo" http://localhost:8080/archivo_prueba1
curl -X PUT -d "datos actualizados" http://localhost:8080/archivo_prueba1
```

### Solicitar un archivo con método HEAD

El método `HEAD` se puede usar para obtener los metadatos de un archivo sin descargar el archivo en sí:

```bash
curl -I http://localhost:8080/archivo_fuente
```

## Notas Adicionales

- El servidor actualmente no implementa todas las características de un servidor web completo y está destinado principalmente para fines educativos y de prueba.
- Asegúrate de que el directorio `resources` contiene los archivos necesarios para las pruebas y que los permisos de archivos son adecuados para que el servidor pueda leer y escribir en ellos.
