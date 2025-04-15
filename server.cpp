#include <iostream>
#include <cstring>      // Para memset()
#include <cstdlib>      // Para atoi()
#include <unistd.h>     // Para close()
#include <netinet/in.h> // Para sockaddr_in
#include <arpa/inet.h>  // Para inet_ntoa()
#include <sys/socket.h> // Para socket, bind, listen, accept

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <PUERTO_HTTP>\n";
        return 1;
    }

    int puerto = atoi(argv[1]);
    cout << "Iniciando servidor en puerto: " << puerto << "...\n";

    int server_fd = socket(PF_INET, SOCK_STREAM, 0); // AF_INET == PF_INET 
    if (server_fd == -1) {
        perror("Error al crear socket");
        return 1;
    }

    sockaddr_in server_addr;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(puerto);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error al hacer bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 10) == -1) {
        perror("Error al escuchar");
        close(server_fd);
        return 1;
    }

    

    cout << "Servidor iniciado y cerrado inmediatamente.\n";
    close(server_fd);

    cout << "Servidor cerrado close () ";
    return 0;
}
