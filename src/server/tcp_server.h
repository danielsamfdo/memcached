#ifndef MEMCACHED_CLONE_BRUTE_FORCE_TCP_SERVER_H
#define MEMCACHED_CLONE_BRUTE_FORCE_TCP_SERVER_H

#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <boost/algorithm/string/predicate.hpp>
#include <common/log_util.h>
#include <common/str_util.h>

using namespace std;

class TCPServer {

private:
    int server_socket;
    struct sockaddr_in socket_address;
    int port;
    int max_conn_backlog = 3;

public:
    TCPServer(int port);
    TCPServer(int port, int max_conn_backlog);
    ~TCPServer();

    void setup();
    void start();
    virtual void process_conn(int socket) = 0;

};

#include "tcp_server.cpp"

#endif //MEMCACHED_CLONE_BRUTE_FORCE_TCP_SERVER_H
