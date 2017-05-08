#ifndef MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_SERVER_H
#define MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_SERVER_H

#include "tcp_server.h"
#include <cache/memcache.h>
#include <cache/lru/lru_cache.h>
#include <cache/rr/random_replacement_cache.h>
#include <cache/landlord/landlord_cache.h>
#include <common/io_util.h>

class MemcacheServer : public TCPServer {

private:
    string suffix;
    shared_ptr<Memcache> cache;
    unsigned long long  size;

public:
    MemcacheServer(unsigned long long  size, int cache_type);
    MemcacheServer(unsigned long long  size, int cache_type, int max_conn_backlog);

    void init(unsigned long long  size, int cache_type);
    void process_conn(int socket);
    string read_command(int socket);
    string process_command(int socket, string command);
    void send_output(int socket, string output);
};

#include "memcache_server.cpp"

#endif //MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_SERVER_H
