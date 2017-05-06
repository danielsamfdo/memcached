#ifndef MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_H
#define MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_H

#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <common/log_util.h>
#include "memcache_element.h"
#include <unordered_map>
#include <common/log_util.h>
#include <common/io_util.h>

using namespace std;

class Memcache {

private:
    unordered_map<string, MemcacheElement> cache;
    MemcacheElement store_fill(vector<string> tokens);

public:
    string process_command(int socket, string command);
    string process_set(int socket, vector<string> tokens);
    string process_add(int socket, vector<string> tokens);
};

#include "memcache.cpp"
#endif //MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_H
