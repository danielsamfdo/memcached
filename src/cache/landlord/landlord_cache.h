#ifndef MEMCACHED_CLONE_BRUTE_FORCE_LANDLORDCACHE_H
#define MEMCACHED_CLONE_BRUTE_FORCE_LANDLORDCACHE_H

#include <cache/memcache.h>
#include <stdlib.h>  

class LandlordCache : public Memcache {

public:
	double long div;
	unordered_map<string, uint64_t> miss_map;
    LandlordCache(uint64_t  size) : Memcache(size)
    {

    	div = 10000;
    }

    
    
	void Cache_miss(string key, uint64_t pt)
	{
		pt /= div;
		miss_map[key] = pt;
	}
    

	void UpdateCache(string key, uint64_t pt)// override
	{
		//Delete the key in the past timestamp
		pt /=div;
		MemcacheElement *e = &cache[key];
		if (e->credit == 0)
		{

			unordered_map<string, uint64_t>::iterator miss_map_iterator;
			miss_map_iterator = miss_map.find(key);
			if (miss_map_iterator != miss_map.end())
			{
				e->credit = pt - miss_map[key];
			}
			else
			{
				e->credit = pt;
			}
			
		}
		else
		{
			double long to_add = rand()%int(pt-e->credit);
			e->credit += to_add;
		}
		
	}
// 
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
			unordered_map<string, MemcacheElement>::iterator cache_iterator;
			// Find delta - 1st iteration
			double long min_delta = cache.begin()->second.credit/cache.begin()->second.bytes;
			for (cache_iterator=cache.begin(); cache_iterator!=cache.end(); ++cache_iterator){

				double long delta = cache_iterator->second.credit/cache_iterator->second.bytes;
				min_delta = min(min_delta, delta);
		  		
		  	}

			// Update credits - 2nd iteration
			for (cache_iterator=cache.begin(); cache_iterator!=cache.end(); ++cache_iterator){

				cache_iterator->second.credit -= min_delta*cache_iterator->second.bytes;

		  	}

			// Delete elements till claimed - 3rd iteration
			vector<string> keys;
			for (cache_iterator=cache.begin(); cache_iterator!=cache.end(); ++cache_iterator){
				if (cache_iterator->second.credit<= 0.0001)
				{
					claimed += cache_iterator->second.bytes;
					keys.push_back(cache_iterator->first);
				}
				if (claimed+avail>=mem_need) break;
		  	}

		  	// Clear keys from cache
		  	for (int i =0; i<keys.size();++i)
		  	{
		  		cache.erase(keys[i]);
		  	}

		}
		memcache_stats.allocated -= claimed;
		//assign the size var to (size-claimed)
		unlockAll();
		return 1;
	}


};

#include "landlord_cache.cpp"
#endif //MEMCACHED_CLONE_BRUTE_FORCE_LANDLORDCACHE_H
