#ifndef MEMCACHED_CLONE_BRUTE_FORCE_LANDLORDCACHE_H
#define MEMCACHED_CLONE_BRUTE_FORCE_LANDLORDCACHE_H

#include <cache/memcache.h>

class LandlordCache : public Memcache {

public:
    LandlordCache(unsigned long long  size) : Memcache(size)
    {
    }
};

#include "landlord_cache.cpp"
#endif //MEMCACHED_CLONE_BRUTE_FORCE_LANDLORDCACHE_H
