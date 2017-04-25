#ifndef MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_H
#define MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_H

#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <common/log_util.h>
#include "memcache_element.h"

using namespace std;

class Memcache {

private:
    unorded_map<string, MemcacheElement> cache;

public:
    string process_command(string command);
};

#include "memcache.cpp"
#endif //MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_H
