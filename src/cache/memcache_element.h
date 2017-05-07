#ifndef MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H
#define MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H

#include <cstdint>

struct MemcacheElement {
    string block;
    uint16_t flags;
    unsigned long long int exptime;
    int bytes;
    uint64_t cas_unique;
};

#endif //MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H
