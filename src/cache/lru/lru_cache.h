#ifndef MEMCACHED_CLONE_BRUTE_FORCE_LRUCACHE_H
#define MEMCACHED_CLONE_BRUTE_FORCE_LRUCACHE_H

#include <cache/memcache.h>
#include <vector>

struct TimeNode
{
	/*
	Struct to keep track of timestamped data in order to evict with O(1)
	*/
	uint64_t ptime;
	TimeNode *next = nullptr;
	vector<string> keys;
};


class LRUCacheElement: public MemElement
{
public:
	TimeNode *lastaccess = nullptr;
	LRUCacheElement();	
};


class LRUCache : public Memcache {
public:
	typedef LRUCacheElement MemcacheElement;
	LRUCache()
	{
		head = nullptr;
		tail = nullptr;
		log_info << "shit got real"<<endl;
	}

// private:

	TimeNode **head;
	TimeNode **tail;

	void UpdateCache(string key,MemcacheElement *e, uint64_t pt)
	{
		//Delete the key in the past timestamp
		log_info << "shit got real" <<endl;
		TimeNode *t = e->lastaccess;
		if (t!=nullptr)
		{
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
		}

		//Make new timestamp and update info there and the tail pointer
		TimeNode *nt  = new TimeNode();
		nt->ptime = get_time();
		nt->keys.push_back(key);
		e->lastaccess = nt;
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
			if (*head == *tail)
			{
				memcache_stats.allocated -= claimed;
				return 0;
			}
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
		memcache_stats.allocated -= claimed;
		//assign the size var to (size-claimed)
		return 1;
	}

};

#include "lru_cache.cpp"
#endif //MEMCACHED_CLONE_BRUTE_FORCE_LRUCACHE_H
