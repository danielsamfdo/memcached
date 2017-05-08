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
#include <mutex>

#include "statistics.h"
using namespace std;
enum OPERATIONS{set=1, add=2, replace=3, append=4, prepend=5, cas=6, get=7, version=8, quit=9, bg_gets=10, del=11, incr=12, decr=13, flush_all=14};

class Memcache {

protected:
    unordered_map<string, MemcacheElement> cache;
    MemcacheElement store_fill(vector<string> tokens);
    unordered_map<string, int > operation;
    static unsigned long long cas_uniq_counter;
    std::mutex Mutexvariables[256]; 

public:
    Memcache(){
        operation.insert(pair<string,int>("set",OPERATIONS::set) );
        operation.insert(pair<string,int>("get",OPERATIONS::get) );
        operation.insert(pair<string,int>("add",OPERATIONS::add) );
        operation.insert(pair<string,int>("replace",OPERATIONS::replace) );
        operation.insert(pair<string,int>("append",OPERATIONS::append) );
        operation.insert(pair<string,int>("prepend",OPERATIONS::prepend) );
        operation.insert(pair<string,int>("cas",OPERATIONS::cas) );
        operation.insert(pair<string,int>("version",OPERATIONS::version) );
        operation.insert(pair<string,int>("quit",OPERATIONS::quit) );
        operation.insert(pair<string,int>("flush_all",OPERATIONS::flush_all) );
        operation.insert(pair<string,int>("gets",OPERATIONS::bg_gets) );
        operation.insert(pair<string,int>("delete",OPERATIONS::del) );
        operation.insert(pair<string,int>("incr",OPERATIONS::incr) );
        operation.insert(pair<string,int>("decr",OPERATIONS::decr) );
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
    string process_get(int socket, vector<string> tokens, bool gets=false);
    string process_gets(int socket, vector<string> tokens);
    string process_delete(int socket, vector<string> tokens);
    string process_incr(int socket, vector<string> tokens);
    string process_decr(int socket, vector<string> tokens);

    // Misc Commands
    string process_version();
    void process_flush_all();
    void process_quit();

    // Other Locking
    void lock(char key);
    void unlock(char key);

};

#include "memcache.cpp"
#endif //MEMCACHED_CLONE_BRUTE_FORCE_MEMCACHE_H
