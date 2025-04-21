# 📡 Servidor HTTP en C++

## 👥 Integrantes
- Juan Pablo Rua Cartagena
- José David Toro Franco
- Sebastian Acosta Molina
- Isabela Osorio Botero

## 📌 Introducción

Este proyecto consiste en la implementación de un servidor HTTP en C++ compatible con HTTP/1.1, capaz de manejar métodos `GET`, `HEAD` y `POST`. El servidor permite servir archivos desde una carpeta específica, gestionar errores HTTP 400 y 404, manejar concurrencia con hilos (threads) y mantener logs de todas las peticiones recibidas.

El servidor fue desplegado y probado en una instancia EC2 de AWS y permite la solicitud de recursos a través de una dirección IP pública.

---

## ⚙️ Desarrollo / Implementación

### 📂 Estructura de Archivos

/home/ubuntu/www/ ├── archivo_1mb.bin ├── caso1.html ├── caso2.html ├── caso3.html ├── caso4.html ├── estilo.css ├── img.jpg ├── img2.jpg ├── index.html ├── script.js ├── video.mp4 └── log.txt


### 📝 Compilación

Para compilar el servidor:

```bash
g++ server.cpp -o server -pthread
```
### 🚀 Ejecución
Para iniciar el servidor:

```bash
./server 8080 /home/ubuntu/www/log.txt /home/ubuntu/www/
```

8080 → puerto HTTP.

/home/ubuntu/www/log.txt → archivo de log.

/home/ubuntu/www/ → carpeta raíz de documentos (DocumentRoot).

### 📑 Funcionalidades Implementadas

✅ Soporte para métodos GET, HEAD, POST.
✅ Manejo de errores HTTP 400 Bad Request y 404 Not Found.
✅ Concurrencia usando std::thread (un hilo por conexión).
✅ Logging de peticiones y respuestas en un archivo.
✅ Servir archivos desde un directorio configurado.
✅ Despliegue exitoso en AWS EC2.

### 📖 Descripción del Código
Inclusión de librerías:
Se incluyen librerías para manejo de sockets, hilos, cadenas y archivos.

Variables globales:
documentRoot y logFile almacenan la ruta base de archivos y logs.

logMessage:
Función para escribir logs en archivo.

parseRequest:
Extrae método, ruta y versión de la petición HTTP.

buildResponse:
Construye una respuesta HTTP con su status, headers y contenido.

readFileContent:
Lee un archivo en binario y devuelve su contenido en string.

handleClient:
Gestiona cada conexión entrante:

Recibe petición.

Parsea método, ruta y versión.

Maneja errores 400 o 404.

Responde según método:

GET: devuelve contenido.

HEAD: solo headers.

POST: recibe y devuelve cuerpo.

Cierra socket.

main:

Valida argumentos.

Crea socket.

Configura SO_REUSEADDR.

Asocia socket a puerto con bind.

Escucha conexiones con listen.

En bucle infinito:

Acepta conexiones.

Crea hilo para cada cliente.

### 🧪 Casos de Prueba
Caso 1: Página con enlaces e imagen
Caso 2: Página con enlaces y video pequeño
Caso 3: Página con múltiples recursos (CSS, JS, imágenes)
Caso 4: Página con un archivo descargable de aproximadamente 1MB

Pruebas realizadas con:
```bash
curl http://18.116.52.123:8080/
curl http://18.116.52.123:8080/caso1.html
curl http://18.116.52.123:8080/caso2.html
curl http://18.116.52.123:8080/caso3.html
curl http://18.116.52.123:8080/caso4.html
```
### Prueba POST:

```bash
curl -X POST http://18.116.52.123:8080/ -d "mensaje=HolaServidor&valor=123"
```

### 🌐 Despliegue en AWS EC2
Instancia creada en EC2 (Ubuntu Server 22.04).

Apertura del puerto 8080 en el Security Group.

Transferencia de archivos vía scp.

Compilación y ejecución del servidor.

Pruebas desde IP pública.

### 📈 Conclusiones
Se logró implementar un servidor HTTP básico y funcional en C++, usando sockets y multihilo.

El servidor puede atender múltiples conexiones simultáneamente gracias a std::thread.

El manejo de errores permite informar al cliente de situaciones como archivos inexistentes o peticiones mal formadas.

El despliegue en AWS EC2 facilitó las pruebas desde distintas herramientas (curl, Postman, navegador).

El uso de logs permitió llevar control de las peticiones recibidas y respuestas enviadas.

### 📚 Referencias
Documentación de sockets en C/C++: Beej's Guide to Network Programming

Documentación oficial de std::thread en C++: https://en.cppreference.com/w/cpp/thread/thread

Protocolo HTTP 1.1: RFC 2616

AWS EC2 User Guide: https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/
### 🎥 Link del video
https://youtu.be/8qSFUlQyOK0
