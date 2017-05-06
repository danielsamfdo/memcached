#include <server/memcache_server.h>
enum CACHE { lru=1, rr, ll };
int main(int arg_count, char const *argv[]) {

    CACHE type_cache = lru;
    MemcacheServer server(type_cache);
    server.setup();
    server.start();
}
