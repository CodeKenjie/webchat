#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// client handlder 
void client_handler(int c_socket) {
    char buffer[BUFFER_SIZE] = {0};
    std::string msg(buffer);
    
    const char* notif = "You are now connected to the server";
    ssize_t serv_notif = send(c_socket, notif, strlen(notif), 0);

    std::cout << buffer << std::endl;

    while (true) {
        ssize_t bytes = read(c_socket, buffer, BUFFER_SIZE-1);
        std::cout << c_socket << ": " << buffer << std::endl;

        if (bytes  < 0) {
            std::cerr << "[error] server cant recieve the msg" << std::endl;
            break;
        }

        if (msg == "quit" || msg == "exit") {
            break;
        }

        std::string reply = "client: " + msg;
        send(c_socket, reply.c_str(), reply.size(), 0);
    }

    std::cout << "Client disconnect" << std::endl;
    close(c_socket);
}

int main() {
    int socket_fd, c_socket;
    int sopt = 1;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == 0) {
        std::cerr << "Socket disfuctional, failed to make" << std::endl;
        exit(1);
    }
    // set socket option 
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &sopt, sizeof(sopt));

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) < 0){
        std::cerr << "Address Failed / Not Supported" << std::endl;
        exit(1);
    }

    // Bind the Socket
    if (bind(socket_fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Binding failed!" << std::endl;
        close(socket_fd);
        exit(1);
    }

    // listen
    if (listen(socket_fd, SOMAXCONN) < 0) {
        std::cerr << "Failed to listen!" << std::endl;
        close(socket_fd);
        exit(1);
    }

    std::cout << "Listening form port: " << PORT << std::endl;

    while (true){
        // take the client addr
        sockaddr_in c_addr;
        socklen_t caddr_len = sizeof(c_addr); 
        // accept incomming connection
        c_socket  = accept(socket_fd, (sockaddr*)&c_addr, &caddr_len);
        
        if (c_socket < 0) {
            std::cerr << "Failed to give a socket to a client!" << std::endl;
            close(c_socket);
            break;
        }

        std::cout << "A client is connected!" << std::endl;

        std::thread(client_handler, c_socket).detach();
    }
    close(c_socket);
    close(socket_fd);

    return 0;
}