#ifndef MEMCACHED_CLONE_BRUTE_FORCE_LRUCACHE_H
#define MEMCACHED_CLONE_BRUTE_FORCE_LRUCACHE_H

#include <cache/memcache.h>
#include <vector>

struct TimeNode
{
	/*
	Struct to keep track of timestamped data in order to evict with O(1)
	*/
	time_t *ptime;
	TimeNode *next = nullptr;
	vector<string> keys;
};


class LRUCacheElement: public MemcacheElement
{
public:
	TimeNode *lastaccess = nullptr;
	//LRUCacheElement();
};


class LRUCache : public Memcache {
public:

	LRUCache(unsigned long long  size) : Memcache(size)
	{
		head = nullptr;
		tail = nullptr;
	}

private:

	TimeNode **head;
	TimeNode **tail;

	void UpdateCache(string key,LRUCacheElement* e, time_t)
	{
		//Delete the key in the past timestamp
		TimeNode *t = e->lastaccess;
		int ind = -1;
		int s = (t->keys).size();
		for(int i=0;i<s;i++)
		{
			if (key == (t->keys)[i])
			{
				ind = i;
				break;
			}
		}
		(t->keys).erase((t->keys).begin()+ind);

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
		//pass
	}

	int Evict(size_t mem_need)
	{
		/*
		Parameters:
		:: mem_need :: Amount of memory needed to evict
		Return:
		:: returns 1 if evicted the needed memory else returns 0
		*/
		size_t claimed = 0;
		while(claimed<mem_need)
		{
			if (*head == *tail) return 0;
			TimeNode *pt = *head;
			int s = (pt->keys).size();
			for(int i=0;i<s;i++)
			{
				string key = pt->keys[i];
				//LRUCacheElement e = ;
				claimed += cache[key].bytes;
				cache.erase(key);
				head = &(pt->next);
			}
		}
		//assign the size var to (size-claimed)
		return 1;
	}

};

#include "lru_cache.cpp"
#endif //MEMCACHED_CLONE_BRUTE_FORCE_LRUCACHE_H
