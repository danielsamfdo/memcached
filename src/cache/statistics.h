#ifndef MEMCACHED_CLONE_BRUTE_FORCE_STATISTICS_H
#define MEMCACHED_CLONE_BRUTE_FORCE_STATISTICS_H

class Statistics {

public:
    int pid, curr_connections, total_connections;
    size_t allocated;
    unsigned long long int start_time;
    unsigned long long int cmd_get, cmd_set, cmd_flush, get_hits, get_misses, delete_misses, delete_hits, incr_misses, incr_hits, decr_misses, decr_hits, cas_misses, cas_hits, reclaimed, bytes_read;
    unsigned long long int limit_maxbytes, evicted_unfetched, expired_unfetched, curr_items, total_items, bytes, version;
    string process_get_statistics(uint64_t ut, string version);
    Statistics()
    {
    	allocated = 0;
        get_hits = 0;
        get_misses = 0;
        incr_hits = 0;
        incr_misses = 0;
        decr_misses = 0;
        decr_hits = 0;
        cas_hits = 0;
        cas_misses = 0;
        delete_hits = 0;
        delete_misses = 0;
        cmd_set = 0;
        cmd_get = 0;
        cmd_flush = 0;
    }
};
#include "statistics.cpp"
#endif