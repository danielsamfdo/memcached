//Ref - http://www.geeksforgeeks.org/socket-programming-cc/
#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <boost/algorithm/string/predicate.hpp>
#include <common/log_util.h>
#include <common/str_util.h>
#define PORT 8080

using namespace std;

int main(int arg_count, char const *argv[]) {

    int server_socket, client_socket, read_len = 0, total_read_len = 0;
    struct sockaddr_in socket_address;
    int addr_len = sizeof(socket_address);
    char buffer[1024] = {0};
    int options = 1;
    int max_read_len = 10;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket == 0) {
        log_error  << "Failed to create server socket" << endl;
        exit(-1);
    }


    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                  &options, sizeof(options))) {
        log_error  << "Failed to set socket options" << endl;
    }

    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(PORT);

    int bind_return = bind(server_socket, (struct sockaddr *)&socket_address,
                           sizeof(socket_address));

    if(bind_return < 0) {
        log_error  << "Failed to bind server" << endl;
    }

    int listen_return = listen(server_socket, 3) ;

    if(listen_return < 0) {
        log_error  << "Failed to listen, I am behra now" << endl;
    }

    log_info << "Server got ears on port " << PORT << ", you got something to say?" << endl;

    while(true) {
        client_socket = accept(server_socket, (struct sockaddr *)&socket_address,
                               (socklen_t*)&addr_len);

        if(client_socket < 0) {
            log_error  << "Failed to hear you, we are not meant for each other, see ya later" << endl;
            break;
        }

        log_info << "Hey there, I got someone to talk to!" << endl;

        string message;
        string suffix("\r\n");

        while(!ends_with(message, suffix)) {
            read_len = read(client_socket, buffer, max_read_len);
            total_read_len += read_len;
            message.append(buffer, read_len);
            log_info << "Currently received message: " << message.c_str() << ", read_len: " << read_len << ", ending ? " << ends_with(message, suffix) << endl;
        }


        log_info << "My new friend says : " << message.c_str() << ", read_len: " << read_len << "\nI will echo it back to him" << endl;
        send(client_socket, message.c_str(), message.size(), 0);
        log_info << "Done" << endl;
    }

    return 0;
}
