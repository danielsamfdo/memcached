#ifndef MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H
#define MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H

#include <cstdint>
#include <mutex>
#include <pthread.h>

class MemcacheElement
 {
 public:
   	string block;
    uint16_t flags;
    time_t exptime;
    size_t bytes;
    uint64_t cas_unique;

 	MemcacheElement()
 	{
 		
 	};

};

#endif //MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H
