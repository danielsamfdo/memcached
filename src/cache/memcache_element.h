#ifndef MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H
#define MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H

#include <cstdint>
#include <mutex>
#include <pthread.h>

class MemcacheElement
 {
 public:
    std::mutex _mutex;
   	string block;
    uint16_t flags;
    time_t exptime;
    size_t bytes;
    uint64_t cas_unique;
 	MemcacheElement()
 	{
 		
 	};

  // http://stackoverflow.com/questions/24998045/c-operator-with-a-mutex-in-class-member  
  MemcacheElement& operator=(const MemcacheElement& o)
  {
      if (this != &o)
      {

        flags = o.flags;
        exptime = o.exptime;
        bytes = o.bytes;
        cas_unique = o.cas_unique;
        block = o.block;
      }
      return *this;
  }

  MemcacheElement (const MemcacheElement &o)
  {
    // std::lock_guard<std::mutex> (o._mutex);
    flags = o.flags;
    exptime = o.exptime;
    bytes = o.bytes;
    cas_unique = o.cas_unique;
    block = o.block;
  }
};

#endif //MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H
