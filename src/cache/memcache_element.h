#ifndef MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H
#define MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H

#include <cstdint>

class MemElement
 {
 public:
 	string block;
    uint16_t flags;
    uint64_t exptime;
    size_t bytes;
    uint64_t cas_unique;
 	MemElement()
 	{
 		
 	};
};

#endif //MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H
