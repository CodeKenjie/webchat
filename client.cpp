#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void recieve_msg(int client_fd) {
    char buffer[BUFFER_SIZE] = {0};

    while (true) {
        ssize_t bytes_recv = recv(client_fd, buffer, sizeof(BUFFER_SIZE)-1, 0);

        if (bytes_recv <= 0) {
            std::cerr << "Disconnected to the server" << std::endl;
            break;
        }
    }
}

int main() {
    int client_fd;
    int opt = 1;
    std::string msg;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd < 0) {
        std::cerr << "Socket Failed" << std::endl;
    }

    setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    sockaddr_in caddr;
    caddr.sin_family = AF_INET;
    caddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &caddr.sin_addr) < 0) {
        std::cerr << "Address Failed / Not Supported" << std::endl;
        close(client_fd);
        exit(1);
    }
    
    if (connect(client_fd, (struct sockaddr*)&caddr, sizeof(caddr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(client_fd);
        exit(1);
    }
    
    std::thread reciever(recieve_msg, client_fd);
    reciever.detach();

    while (true) {

        std::cout << "[Message]: ";
        std::getline(std::cin, msg);

        if (msg == "quit" || msg == "exit") {
            return false;
            break;
        }

        ssize_t bytes_sent = send(client_fd, msg.c_str(), msg.size(), 0);

        if (bytes_sent < 0) {
            std::cerr << "cant send a messege to the server" << std::endl;
            break;        
        }

    }

    close (client_fd);
    return 0;
}
