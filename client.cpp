#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsa;
    SOCKET socket_client;
    struct sockaddr_in direccio_servidor;
    char buffer[1024];

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cerr << "Error en inicializar Winsock.\n";
        return 1;
    }

    socket_client = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_client == INVALID_SOCKET) {
        cerr << "Error en crear el socket del cliente.\n";
        return 1;
    }

    direccio_servidor.sin_family = AF_INET;
    direccio_servidor.sin_port = htons(9100);
    direccio_servidor.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(socket_client, (struct sockaddr*)&direccio_servidor, sizeof(direccio_servidor)) == SOCKET_ERROR) {
        cerr << "Error en conectar con el servicio.\n";
        return 1;
    }

    cout << "Conexión establecida con el servicio.\n";

    string comanda;
    cout << "Introduzca su comando: ";
    getline(cin, comanda);

    send(socket_client, comanda.c_str(), comanda.length(), 0);

    int bytes_rebuts = recv(socket_client, buffer, 1024, 0);
    if (bytes_rebuts < 0) {
        cerr << "Error en recibir la respuesta del servicio.\n";
        return 1;
    }

    cout << "Identificador recibido: " << buffer << "\n";

    closesocket(socket_client);
    WSACleanup();
    return 0;
}