#ifndef MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_H
#define MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_H

#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <common/log_util.h>
#include "memcache_element.h"
#include <unordered_map>
#include <common/log_util.h>
#include <common/io_util.h>

#include "statistics.h"
using namespace std;
enum OPERATIONS{set=1, add, replace, append, prepend, cas, get};

class Memcache {

private:
    unordered_map<string, MemcacheElement> cache;
    MemcacheElement store_fill(vector<string> tokens);
    unordered_map<string, int > operation;
    static unsigned long long cas_uniq_counter;

public:
    Memcache(){
        operation.insert(pair<string,int>("set",OPERATIONS::set) );
        operation.insert(pair<string,int>("get",OPERATIONS::get) );
        operation.insert(pair<string,int>("add",OPERATIONS::add) );
        operation.insert(pair<string,int>("replace",OPERATIONS::replace) );
        operation.insert(pair<string,int>("append",OPERATIONS::append) );
        operation.insert(pair<string,int>("prepend",OPERATIONS::prepend) );
        operation.insert(pair<string,int>("cas",OPERATIONS::cas) );
        // cas_uniq_counter = 0 ;
    }
    Statistics memcache_stats;
    static unsigned long long get_cas_counter();
    string process_command(int socket, string command);
    // Storage commands
    string process_set(int socket, vector<string> tokens);
    string process_replace(int socket, vector<string> tokens);
    string process_append(int socket, vector<string> tokens);
    string process_prepend(int socket, vector<string> tokens);
    string process_add(int socket, vector<string> tokens);
    string process_cas(int socket, vector<string> tokens);

    // Retrieval commands
    string process_get(int socket, vector<string> tokens);
};

#include "memcache.cpp"
#endif //MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_H
