#include <server/memcache_server.h>
enum CACHE { lru=1, rr, ll };
int main(int arg_count, char const *argv[]) {
    int port = 8080;
    CACHE type_cache = lru;
    MemcacheServer server(type_cache, port);
    server.setup();
    server.start();
}
