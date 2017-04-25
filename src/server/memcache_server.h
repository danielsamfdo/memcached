#ifndef MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_SERVER_H
#define MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_SERVER_H

#include "tcp_server.h"
#include <cache/memcache.h>

class MemcacheServer : public TCPServer {

private:
    int max_read_len;
    string suffix;
    Memcache cache;

public:
    MemcacheServer(int cache_type, int port);
    MemcacheServer(int cache_type, int port, int max_conn_backlog);

    void init();
    void process_conn(int socket);
    string read_command(int socket);
    string process_command(int socket);
    void send_output(int socket, string output);
};

#include "memcache_server.cpp"

#endif //MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_SERVER_H
