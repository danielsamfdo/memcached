//Ref - http://www.geeksforgeeks.org/socket-programming-cc/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <boost/algorithm/string/predicate.hpp>
#define PORT 8080

int main(int arg_count, char const *argv[]) {

    int client_socket, read_len, connect_return;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};
    std::string client_message ("Hello from client! Mujhse fraandship karoge ?\r\n");

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(client_socket < 0) {
        perror("Failed to create client socket");
        exit(-1);
    }

    memset(&server_address, '0', sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Address is invalid or not supported");
        exit(-1);
    }

    connect_return = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    if(connect_return < 0) {
        perror("Could not connect to server, maybe we're not made for each other");
        exit(-1);
    }

    printf("Connected to server, telling him %s\n", client_message.c_str());
    send(client_socket, client_message.c_str(), client_message.size(), 0);
    printf("Done, waiting to hear back\n");

    std::string message;
    std::string suffix("\r\n");

    //while(!boost::algorithm::ends_with(message, suffix)) {
        read_len = read(client_socket, buffer, 1024);
        //message.append(buffer);
    //}

    printf("Server said : %s\n", buffer);
    printf("Time to die\n");

    return 0;
}

