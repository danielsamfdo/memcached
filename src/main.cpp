#include <server/memcache_server.h>
#include <common/log_util.h>

enum CACHE { lru=1, rr, ll };
int main(int arg_count, char const *argv[]) {

    if(arg_count < 2) {
        log_error << "Usage: " << argv[0] << " <size[k|m|g]> [--lru | --random | --landlord]" << endl;
        return -1;
    }

    unsigned long long size = 1l;
    string size_str(argv[1]);

    if(size_str.back() == 'g') {
        size = 1024l *1024l *1024l;
        size_str.pop_back();
    }
    else if(size_str.back() == 'm') {
        size = 1024l * 1024l;
        size_str.pop_back();
    }
    else if(size_str.back() == 'k') {
        size = 1024l;
        size_str.pop_back();
    }
    else if(!isdigit(size_str.back())) {
        log_error << "Usage: " << argv[0] << " <size[k|m|g]> [--lru | --random | --landlord]" << endl;
        return -1;
    }

    try {
        unsigned long long num = stoll(size_str);
        size *= num;
    }
    catch (exception& e) {
        log_error << "Exception " << e.what() << endl;
        log_error << "Usage: " << argv[0] << " <size[k|m|g]> [--lru | --random | --landlord]" << endl;
        return -1;
    }

    CACHE type_cache = lru;
    if(arg_count >= 3) {
        string type_str(argv[2]);

        if(type_str == "--lru") {
            type_cache = lru;
        }
        else if(type_str == "--random") {
            type_cache = rr;
        }
        else if(type_str == "--landlord") {
            type_cache = ll;
        }
        else {
            log_error << "Usage: " << argv[0] << " <size[k|m|g]> [--lru | --random | --landlord]" << endl;
            return -1;
        }
    }

    log_info << "Running memcache with size " << size << " cache type " << type_cache << endl;

    MemcacheServer server(size, type_cache);
    server.setup();
    server.start();
}
