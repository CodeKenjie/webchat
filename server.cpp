/*  This is For Educational purposes only 
            NETWORK PROGRAMMING (TCP)
    -to learn or understand how the networking program function

    -socket - to set what net you are using like AF_NET = IPv4 or AF_NET6 = IPv6, your SOCK_STREAM or SOCK_DSTREAM, and 0 for TCP=stdin, 1 for stdout, 2 for stderr and 3 for open files;
    -port - to identify the family_t, port and the address
    -you can also set a port option using setportopt (init socket, init level, int optname, const void *optval, socklen_t optlen);
    -then bind the socket and port
    -
*/

#include <iostream>
//libraries needed for the NETWORK Programming
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock, n_sock;
    int opt = 1;
    char buffer[BUFFER_SIZE] = {0};

    // this is the socket i have created
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == 0) {
        perror("Socket Failed");
        exit(1);
    }

    // set socket options (optional) (socket option reuse addr or socket addr reuse port)
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    //inside a sockaddr_in setting port value
    // declaring the sockaddr_in for making a port ---its basically a port that you can set;
    sockaddr_in saddr;
    int addrlen = sizeof(saddr);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr); // setting a string sin_addr to a binary, for security purposes;;
    //sin_zero --only here for compatibility;
    
    
    // binding socket to a port
    //the struct sockaddr contains sa_family
    // this statement binds the everything
    if (bind(sock, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        perror("Failed to Bind");
        close(sock);
        exit(1);
    }

    //listen for connections
    //assign socket and listen, setting listen queue to 3 (can be greater than 3) this is just a maximum pending queue, and if it reaches 0 then fail the listen and exit
    if (listen(sock, 1) < 0) {
        perror("Listened");
        close(sock);
        exit(1);
    }

    std::cout << "Server listening on port: " << PORT << std::endl;

    // when a client connect accept it and it give it a new socket
    //accept incoming connection
    sockaddr_in caddr;
    socklen_t client_len = sizeof(caddr);

    n_sock = accept(sock, (struct sockaddr*)&caddr, &client_len);

    if (n_sock < 0) {
        perror("Accept");
        close(n_sock);
        exit(1);
    }

    std::cout << "A client successfully connected!" << std::endl;

    // read data from client
    read(n_sock, buffer, BUFFER_SIZE);
    std::cout << "Messege from the client: " << buffer << std::endl;
    //send response
    const char* response = "Connected to the server;";
    send (n_sock, response, strlen(response), 0);

    std::cout << "Response sent\n";

    // then close the sockets
    close (n_sock);
    close (sock);

    return 0;
}