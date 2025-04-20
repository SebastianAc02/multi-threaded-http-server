#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

string documentRoot;
string logFile;

void logMessage(const string& message) {
    ofstream log(logFile, ios::app);
    if (log.is_open()) {
        log << message << endl;
    }
}

void parseRequest(const string& request, string& method, string& path, string& version) {
    istringstream requestStream(request);
    requestStream >> method >> path >> version;
}

string buildResponse(int statusCode, const string& content = "", const string& contentType = "text/html") {
    string statusText;
    switch (statusCode) {
        case 200: statusText = "OK"; break;
        case 400: statusText = "Bad Request"; break;
        case 404: statusText = "Not Found"; break;
        default: statusText = "Internal Server Error";
    }

    stringstream response;
    response << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n";
    response << "Content-Type: " << contentType << "\r\n";
    response << "Content-Length: " << content.size() << "\r\n";
    response << "Connection: close\r\n\r\n";

    if (statusCode == 200 && !content.empty()) {
        response << content;
    }

    return response.str();
}

string readFileContent(const string& path) {
    ifstream file(path, ios::binary);
    if (file) {
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    return "";
}

void handleClient(int clientSocket) {
    char buffer[8192];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        close(clientSocket);
        return;
    }

    buffer[bytesReceived] = '\0';
    string request(buffer);
    cout << "Petición recibida:\n" << request << endl;
    logMessage("Petición:\n" + request);

    string method, path, version;
    parseRequest(request, method, path, version);

    cout << "Método: " << method << " | Ruta: " << path << " | Versión: " << version << endl;
    logMessage("Método: " + method + " | Ruta: " + path + " | Versión: " + version);

    if (version != "HTTP/1.1") {
        string response = buildResponse(400, "<h1>400 Bad Request</h1>");
        send(clientSocket, response.c_str(), response.size(), 0);
        logMessage("Respuesta: 400 Bad Request");
    } else if (method == "GET" || method == "HEAD") {
        string filePath = documentRoot + path;
        if (path == "/") filePath += "index.html";

        string content = readFileContent(filePath);
        if (content.empty()) {
            string response = buildResponse(404, "<h1>404 Not Found</h1>");
            send(clientSocket, response.c_str(), response.size(), 0);
            logMessage("Respuesta: 404 Not Found");
        } else {
            string response = buildResponse(200, method == "GET" ? content : "");
            send(clientSocket, response.c_str(), response.size(), 0);
            logMessage("Respuesta: 200 OK");
        }
    } else if (method == "POST") {
        size_t bodyPos = request.find("\r\n\r\n");
        string body = (bodyPos != string::npos) ? request.substr(bodyPos + 4) : "";
        cout << "Datos POST:\n" << body << endl;
        logMessage("Datos POST:\n" + body);

        string response = buildResponse(200, "<h1>POST recibido</h1><pre>" + body + "</pre>");
        send(clientSocket, response.c_str(), response.size(), 0);
        logMessage("Respuesta: 200 OK POST");
    } else {
        string response = buildResponse(400, "<h1>400 Bad Request</h1>");
        send(clientSocket, response.c_str(), response.size(), 0);
        logMessage("Respuesta: 400 Bad Request");
    }

    close(clientSocket);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Uso: " << argv[0] << " <PUERTO_HTTP> <LogFile> <DocumentRootFolder>\n";
        return 1;
    }

    int puerto = atoi(argv[1]);
    logFile = argv[2];
    documentRoot = argv[3];
    if (documentRoot.back() != '/') documentRoot += "/";

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        cerr << "Error al crear socket\n";
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(puerto);

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Error en bind\n";
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        cerr << "Error en listen\n";
        close(server_fd);
        return 1;
    }

    cout << "Servidor escuchando en el puerto " << puerto << "...\n";

    while (true) {
        sockaddr_in client_addr;
        socklen_t client_size = sizeof(client_addr);
        int clientSocket = accept(server_fd, (sockaddr*)&client_addr, &client_size);
        if (clientSocket < 0) {
            cerr << "Error en accept\n";
            continue;
        }

        cout << "Conexión aceptada desde: " << inet_ntoa(client_addr.sin_addr) << endl;
        thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }

    close(server_fd);
    return 0;
}
