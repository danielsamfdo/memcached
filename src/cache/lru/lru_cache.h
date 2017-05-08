#ifndef MEMCACHED_CLONE_BRUTE_FORCE_LRUCACHE_H
#define MEMCACHED_CLONE_BRUTE_FORCE_LRUCACHE_H

#include <cache/memcache.h>
#include <vector>

/*struct TimeNode
{
	
	Struct to keep track of timestamped data in order to evict with O(1)
	
	uint64_t ptime;
	TimeNode *next = nullptr;
	vector<string> keys;
};*/





class LRUCache : public Memcache {
public:
	// typedef LRUCacheElement MemcacheElement;
	TimeNode *head;
	TimeNode *tail;
	// TimeNode *tmp1;
	LRUCache(uint64_t  size) : Memcache(size)
	{
		head = nullptr;
		tail = nullptr;
		log_info << " LRU CONSTRUCTOR "<<endl;
	}

// private:


	virtual void Clear_CacheElement(string key)
	{
		MemcacheElement *e = &cache[key];
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

	}
    virtual void Clear_CacheAll()
    {
    	TimeNode *tp = head;
    	TimeNode *tp2 = nullptr;
    	head = nullptr;
    	tail = nullptr;
    	while(tp!=nullptr)
    	{
    		tp2 = tp;
    		tp = tp->next;
    		delete tp2;
    	}
    }
    

	void UpdateCache(string key, uint64_t pt)// override
	{
		//Delete the key in the past timestamp
		MemcacheElement *e = &cache[key];
		Clear_CacheElement(key);

		// Make new timestamp and update info there and the tail pointer
		TimeNode *nt  = new TimeNode();
		
		nt->ptime = get_time();
		nt->keys.push_back(key);
		e->lastaccess = nt;
		TimeNode *tmp = nt;
		while(tmp!=nullptr)
		{
			log_info << "Cic"<<tmp->ptime << endl;
			tmp = tmp->next;
		}
		if (head==nullptr)
		{
			head = nt;
			tail = nt;
			// tmp1 = nt;
			nt->next = nullptr;
			// log_info << "shit got real  1&&&&&&&&&&&&&&&&&&&&&&" <<nt->ptime<<endl;
			tmp = nt;
			while(tmp!=nullptr)
			{
				log_info << tmp->ptime << endl;
				tmp = tmp->next;
			}
		}
		else
		{
			// log_info<<(*head)->ptime<<endl;
			(tail)->next = nt;
			// log_info << "shit got real  2&&&&&&&&&&&&&&&&&&&&&&" << (*tail)->ptime<<endl;
			tail = nt;
			nt->next = nullptr;
			// log_info << "shit got real  3&&&&&&&&&&&&&&&&&&&&&&" << nt->ptime<<endl;
		}
		// log_info<<tmp1->ptime<<endl;
		
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

			if (head == nullptr)
			{			
				tail = nullptr;
				memcache_stats.allocated -= claimed;
				// log_info << "shit got real2 &&&&&&&&&&&&&&&&&&&&&&&" << claimed << " " << (*head)->ptime<<endl;
				return 0;
			}
			// log_info << "shit got real2 &&&&&&&&&&&&&&&&&&&&&&&" <<endl;
			TimeNode *pt = head;
			int s = (pt->keys).size();
			for(int i=0;i<s;i++)
			{
				string key = pt->keys[i];
				//LRUCacheElement e = ;
				claimed += cache[key].bytes;
				cache.erase(key);
			}
			head = (pt->next);

			log_info << "shit got real2 &&&&&&&&&&&&&&&&&&&&&&&" <<endl;
		}
		memcache_stats.allocated -= claimed;
		//assign the size var to (size-claimed)
		unlockAll();
		return 1;
	}

};

#include "lru_cache.cpp"
#endif //MEMCACHED_CLONE_BRUTE_FORCE_LRUCACHE_H
