#ifndef MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H
#define MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H

#include <cstdint>

struct MemcacheElement {
    shared_ptr<const char> block;
    uint16_t flags;
    int exptime;
    int bytes;
    uint64_t cas_unique;
};

#endif //MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H
