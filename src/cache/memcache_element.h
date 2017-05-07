#ifndef MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H
#define MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H

#include <cstdint>

struct TimeNode
{
	/*
	Struct to keep track of timestamped data in order to evict with O(1)
	*/
	uint64_t ptime;
	TimeNode *next = nullptr;
	vector<string> keys;
};

class MemcacheElement
 {
 public:
 	string block;
    uint16_t flags;
    uint64_t exptime;
    size_t bytes;
    uint64_t cas_unique;
    TimeNode *lastaccess;
 	MemcacheElement()
 	{
 		lastaccess = nullptr;
 	};
};

#endif //MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_ELEMENT_H
