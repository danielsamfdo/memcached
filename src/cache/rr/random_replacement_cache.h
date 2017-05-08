#ifndef MEMCACHED_CLONE_BRUTE_FORCE_RANDOMREPLACEMENTCACHE_H
#define MEMCACHED_CLONE_BRUTE_FORCE_RANDOMREPLACEMENTCACHE_H

#include <cache/memcache.h>

class RandomReplacementCache : public Memcache {

public:

    RandomReplacementCache(unsigned long long  size) : Memcache(size)
	{
		log_info << " RR CONSTRUCTOR "<<endl;
	}

// private:
// 
	void UpdateCache(string key, MemcacheElement *e, uint64_t pt){}
	int Evict(uint64_t mem_need)
	{
		/*
		Parameters:
		:: mem_need :: Amount of memory needed to evict
		Return:
		:: returns 1 if evicted the needed memory else returns 0
		*/

		lockAll();
		log_info<<"In Evict"<<endl;
		uint64_t claimed = 0;
		uint64_t avail = capacity - memcache_stats.allocated;
		while(claimed+avail<mem_need)
		{
			if (cache.size()>0)
			{
				claimed += (cache.begin()->second).bytes;
				log_info << "Evicting from RR"<< cache.begin()->first << endl;
				cache.erase(cache.begin());
			}
			else
			{
				unlockAll();
				return 0;
			}
		}
		memcache_stats.allocated -= claimed;
		log_info << memcache_stats.allocated << "  " << claimed<<endl;
		//assign the size var to (size-claimed)
		unlockAll();
		return 1;
	}

};

#include "random_replacement_cache.cpp"
#endif //MEMCACHED_CLONE_BRUTE_FORCE_RANDOMREPLACEMENTCACHE_H
