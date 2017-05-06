//For reference only, Ref - http://www.geeksforgeeks.org/socket-programming-cc/
#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <boost/algorithm/string/predicate.hpp>
#include <common/log_util.h>
#define PORT 8080

using namespace std;

int main(int arg_count, char const *argv[]) {

    int client_socket, read_len, connect_return;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};
    string client_message ("Hello from client! Mujhse fraandship karoge ?\r\n");

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(client_socket < 0) {
        log_error  << "Failed to create client socket" << endl;
        exit(-1);
    }

    memset(&server_address, '0', sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        log_error  << "Address is invalid or not supported" << endl;
        exit(-1);
    }

    connect_return = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    if(connect_return < 0) {
        log_error  << "Could not connect to server, maybe we're not made for each other" << endl;
        exit(-1);
    }

    log_info << "Connected to server, telling him " << client_message.c_str() << endl;
    send(client_socket, client_message.c_str(), client_message.size(), 0);
    log_info << "Done, waiting to hear back" << endl;

    string message;
    string suffix("\r\n");

    //while(!boost::algorithm::ends_with(message, suffix)) {
        read_len = read(client_socket, buffer, 1024);
        //message.append(buffer);
    //}

    log_info << "Server said : " << buffer << endl;
    log_info << "Time to die" << endl;

    return 0;
}

