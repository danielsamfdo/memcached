#include <thread>
#include <errno.h>
#include <string.h>

TCPServer::TCPServer() {
}

TCPServer::TCPServer(int max_conn_backlog) {
    this->max_conn_backlog = max_conn_backlog;
}

TCPServer::~TCPServer() {

}

void TCPServer::setup() {

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket == 0) {
        log_error  << "Failed to create server socket" << endl;
        exit(-1);
    }

    log_info << "Server Socket "<< server_socket <<endl;

    int options = 1;
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT,
                   &options, sizeof(options))) {
        log_error << "Oh dear, something went wrong with setting socket options()! :: " << strerror(errno) << endl;
        log_error  << "Failed to set socket options" << endl;
        exit(-1);
    }

    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(PORT);

    int bind_return = ::bind(server_socket, (struct sockaddr *)&socket_address,
                           sizeof(socket_address));

    if(bind_return < 0) {
        log_error << "Oh dear, something went wrong with binding server()! :: " << strerror(errno) << endl;
        log_error  << "Failed to bind server" << endl;
        exit(-1);
    }

    int listen_return = listen(server_socket, max_conn_backlog) ;

    if(listen_return < 0) {
        log_error  << "Failed to listen" << endl;
        exit(-1);
    }

    log_info << "Server is up and listening on PORT" << endl;
}

void TCPServer::start() {

    int addr_len = sizeof(socket_address);

    while (true) {
        int client_socket = accept(server_socket, (struct sockaddr *)&socket_address,
                                   (socklen_t*)&addr_len);

        if(client_socket < 0) {
            log_error  << "Failed to hear you, we are not meant for each other, see ya later" << endl;
            break;
        }

        log_info << "New client connected, processing." << endl;

        function<void(int)> fun_ptr = [&](int sock) { this->process_conn(sock); };
        thread client_thread(fun_ptr, client_socket);
        client_thread.detach(); //you're on your own now, threadyboy
    }
}