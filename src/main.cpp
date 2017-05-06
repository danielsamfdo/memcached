#include <server/memcache_server.h>

int main(int arg_count, char const *argv[]) {

    MemcacheServer server(1);
    server.setup();
    server.start();
}
