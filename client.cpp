/* Client side Programming 
    This is For Educational purposes only 
            NETWORK PROGRAMMING (TCP)
    -to learn or understand how the networking program function


*/
#include <iostream>
// tipical networking libraries
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    int opt = 1;
    char buffer[BUFFER_SIZE] = {0}; // all elements zero -initalized


    //create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    //check if the creation of the socket is successful
    if (sock < 0){
        std::cerr << "Socket Creation failed!" << std::endl;
        exit(1);
    }
    // set a socket option
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // Set the value of the port and the port address
    sockaddr_in saddr;
    int addrlen = sizeof(saddr);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    // check if the inet_pton is working
    if (inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr) < 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        exit(1);
    }

    // stablish a connection to a listening sever
    if (connect(sock, (struct sockaddr*)&saddr, sizeof(saddr)) < 0){
        std::cerr << "Connection Failed" << std::endl;
        close(sock);
        exit(1);
    }

    //std::cout << "Client connected to a server" << std::endl;

    //send messege

    const char* hello = "Hello from client";
    ssize_t bytes_sent = send(sock, hello, strlen(hello), 0);

    if (bytes_sent < 0) {
        std::cerr << "Failed to send messege" << std::endl;
        close(sock);   
        exit(1);
    }

    std::cout << "Message sent: " << std::endl;

    // recieve response
    ssize_t smsg = recv(sock, buffer, BUFFER_SIZE, 0);

    if (smsg < 0) {
        std::cerr << "Recieve Failed" << std::endl;
        close(sock);
        exit(1);
    }
    
    buffer[smsg] = '\0';
    std::cout << "Recieved: " << buffer << std::endl;


    close(sock);
    return 0;
}