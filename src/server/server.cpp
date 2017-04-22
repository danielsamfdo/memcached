//Ref - http://www.geeksforgeeks.org/socket-programming-cc/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <boost/algorithm/string/predicate.hpp>
#define PORT 8080

int main(int arg_count, char const *argv[]) {

    int server_socket, client_socket, read_len = 0, total_read_len = 0;
    struct sockaddr_in socket_address;
    int addr_len = sizeof(socket_address);
    char buffer[1024] = {0};
    int options = 1;
    int max_read_len = 10;

    std::string server_message("Hello from client! Mujhse fraandship karoge ?\\r\\n");

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket == 0) {
        perror("Failed to create server socket");
        exit(-1);
    }

    /*if(fcntl(server_socket, F_SETFL, fcntl(server_socket, F_GETFL) | O_NONBLOCK) < 0) {
        perror("Failed to set server socket to non-blocking");
        exit(-1);
    }*/

    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                  &options, sizeof(options))) {
        perror("Failed to set socket options");
        exit(-1);
    }

    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(PORT);

    int bind_return = bind(server_socket, (struct sockaddr *)&socket_address,
                           sizeof(socket_address));

    if(bind_return < 0) {
        perror("Failed to bind server");
    }
    printf("Server got ears on port %d, you got something to say?\n", PORT);

    int listen_return = listen(server_socket, 3) ;

    if(listen_return < 0) {
        perror("Failed to listen, I am behra now");
    }



    printf("Server got ears on port %d, you got something to say?\n", PORT);

    while(true) {
        client_socket = accept(server_socket, (struct sockaddr *)&socket_address,
                               (socklen_t*)&addr_len);

        if(client_socket < 0) {
            perror("Failed to hear you, we are not meant for each other, see ya later");
            break;
        }

        printf("Hey there, I got someone to talk to!\n");

        std::string message;
        std::string suffix("\r\n");

        while(!boost::algorithm::ends_with(message, suffix)) {
            read_len = read(client_socket, buffer, max_read_len);
            total_read_len += read_len;
            message.append(buffer, read_len);
            //memset(buffer, '0', sizeof(buffer));
            printf("Currently received message:%s, read_len:%d, ending ? %d\n", message.c_str(), read_len, boost::algorithm::ends_with(message.c_str(), suffix.c_str()));
        }


        printf("My new friend says : %s, read_len: %d\nI will echo it back to him\n", message.c_str(), read_len);
        send(client_socket, message.c_str(), message.size(), 0);
        printf("Done\n");
    }

    return 0;
}
