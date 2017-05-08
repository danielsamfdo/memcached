MemcacheServer::MemcacheServer(unsigned long long  size, int cache_type) : TCPServer() {
    init(size, cache_type);
}

MemcacheServer::MemcacheServer(unsigned long long  size, int cache_type, int max_conn_backlog) : TCPServer(max_conn_backlog) {
    init(size, cache_type);
}


void MemcacheServer::init(unsigned long long  size, int cache_type) {
    suffix = "\r\n";
    this->size = size;

    switch(cache_type) {
        case 1: {
            shared_ptr<LRUCache> lru = make_shared<LRUCache>(size);
            log_info << "Starting LRU CACHE MEMCACHED " << endl;
            cache = lru;
            break;
        }
        case 2: {
            shared_ptr<RandomReplacementCache> rr = make_shared<RandomReplacementCache>(size);
            cache = rr;
            break;
        }
        case 3: {
            shared_ptr<LandlordCache> ll = make_shared<LandlordCache>(size);
            cache = ll;
            break;
        }
    }
}

void MemcacheServer::process_conn(int socket) {

    while(true) {
        string command = read_command(socket);
        string output = process_command(socket, command);

        if(! output.empty()) {
            send_output(socket, output);
        }
    }
}

string MemcacheServer::read_command(int socket) {

    return read_suffix(socket, suffix);
}

string MemcacheServer::process_command(int socket, string command) {

    return cache->process_command(socket, command);
}

void MemcacheServer::send_output(int socket, string output) {

    output.append(suffix);
    send(socket, output.c_str(), output.size(), 0);
}
