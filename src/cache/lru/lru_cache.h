#ifndef MEMCACHED_CLONE_BRUTE_FORCE_LRUCACHE_H
#define MEMCACHED_CLONE_BRUTE_FORCE_LRUCACHE_H

#include <cache/memcache.h>
#include <vector>

struct TimeNode
{
	time_t *ptime;
	TimeNode *next = nullptr;
	vector<string> keys;
};


class LRUCacheElement: public MemcacheElement
{
public:
	TimeNode *lastaccess = nullptr;
	LRUCacheElement();	
};


class LRUCache : public Memcache {
public:
	TimeNode **head;
	TimeNode **tail;

	LRUCache()
	{
		head = nullptr;
		tail = nullptr;
	}

	void UpdateCache(string key,MemcacheElement* e, time_t)
	{
		//Delete the key in the past timestamp
		TimeNode *t = e->lastaccess;
		int ind = -1;
		for(int i=0;i<(t->keys).size();i++)
		{
			if (key == (t->keys)[i])
			{
				ind = i;
				break;
			}
		}
		(t->keys).erase((t->keys).begin()+ind)

		//Make new timestamp and update info there and the tail pointer
		TimeNode *nt  = new TimeNode();
		nt->keys.push_back(key);
		if (head==nullptr)
		{
			head = &nt;
			tail = &nt;
		}
		else
		{
			(*tail)->next = nt;
			tail = &nt;
		}
		pass
	}

	void Evict()

};

#include "lru_cache.cpp"
#endif //MEMCACHED_CLONE_BRUTE_FORCE_LRUCACHE_H
