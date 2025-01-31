#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

mutex comandes_mutex;
int contador_comandes = 1;

string generar_identificador_comanda() {
    stringstream ss;
    ss << "ORDRE-" << setw(4) << setfill('0') << contador_comandes++;
    return ss.str();
}

void guardar_comanda(const string& identificador, const string& comanda) {
    lock_guard<mutex> lock(comandes_mutex);
    ofstream archivo("comandes.txt", ios::app);
    if (archivo.is_open()) {
        archivo << identificador << ": \"" << comanda << "\"\n";
        archivo.close();
    } else {
        cerr << "Error al abrir el archivo de comandos.\n";
    }
}

void gestionar_cliente(SOCKET socket_cliente) {
    char buffer[1024];
    string comanda;
    int bytes_recibidos;

    while ((bytes_recibidos = recv(socket_cliente, buffer, 1024, 0)) > 0) {
        comanda.append(buffer, bytes_recibidos);
        if (comanda.find('\n') != string::npos) {
            break;  // Hemos recibido una línea completa
        }
    }

    string identificador = generar_identificador_comanda();
    guardar_comanda(identificador, comanda);

    // Envía el identificador incluyendo el carácter nulo al final
    send(socket_cliente, identificador.c_str(), identificador.length() + 1, 0); 
    closesocket(socket_cliente);
}

int main() {
    WSADATA wsa;
    SOCKET socket_servidor, socket_cliente;
    sockaddr_in direccion_servidor, direccion_cliente;
    int longitud_cliente;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cerr << "Error al inicializar Winsock.\n";
        return 1;
    }

    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_servidor == INVALID_SOCKET) {
        cerr << "Error al crear el socket del servidor.\n";
        return 1;
    }

    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_port = htons(9100);
    direccion_servidor.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_servidor, (struct sockaddr*)&direccion_servidor, sizeof(direccion_servidor)) == SOCKET_ERROR) {
        cerr << "Error al enlazar el socket del servidor.\n";
        return 1;
    }

    if (listen(socket_servidor, 10) == SOCKET_ERROR) {
        cerr << "Error al poner el servidor a escuchar.\n";
        return 1;
    }

    cout << "Servicio iniciado. Escuchando en el puerto 9100...\n";

    while (true) {
        longitud_cliente = sizeof(direccion_cliente);
        socket_cliente = accept(socket_servidor, (struct sockaddr*)&direccion_cliente, &longitud_cliente);
        if (socket_cliente == INVALID_SOCKET) {
            cerr << "Error al aceptar la conexión del cliente.\n";
            continue;
        }

        cout << "Conexión aceptada.\n";

        thread(gestionar_cliente, socket_cliente).detach();
    }

    closesocket(socket_servidor);
    WSACleanup();
    return 0;
}