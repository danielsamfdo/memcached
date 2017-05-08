#include <server/memcache_server.h>
enum CACHE { lru=1, rr, ll };
int main(int arg_count, char const *argv[]) {

    CACHE type_cache = lru;
    MemcacheServer server(2);
    server.setup();
    server.start();
}
