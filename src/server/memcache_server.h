#ifndef MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_SERVER_H
#define MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_SERVER_H

#include "tcp_server.h"

class MemcacheServer : public TCPServer {

public:
    MemcacheServer(int port);
    MemcacheServer(int port, int max_conn_backlog);

    void process_conn(int socket);
};

#include "memcache_server.cpp"

#endif //MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_SERVER_H
