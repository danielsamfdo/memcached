#include <server/memcache_server.h>

int main(int arg_count, char const *argv[]) {

    int port = 8080;

    MemcacheServer server(port, 1);
}
