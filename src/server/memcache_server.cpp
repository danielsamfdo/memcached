MemcacheServer::MemcacheServer(int cache_type, int port) : TCPServer(port) {
    init(cache_type);
}

MemcacheServer::MemcacheServer(int cache_type, int port, int max_conn_backlog) : TCPServer(port, max_conn_backlog) {
    init(cache_type);
}

void MemcacheServer::init(int cache_type) {
    max_read_len = 10;
    suffix = "\r\n";

    switch(cache_type) {
        case 1: {
            shared_ptr<LRUCache> lru = make_shared<LRUCache>();
            cache = lru;
            break;
        }
        case 2: {
            shared_ptr<RandomReplacementCache> rr = make_shared<RandomReplacementCache>();
            cache = rr;
            break;
        }
        case 3: {
            shared_ptr<LandlordCache> ll = make_shared<LandlordCache>();
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

    string command;
    char buffer[max_read_len];
    memset( buffer, 0, max_read_len*sizeof(char) );

    while(!ends_with(command, suffix)) {
        int read_len = read(socket, buffer, max_read_len);
        command.append(buffer, read_len);
    }

    return command;
}

string MemcacheServer::process_command(int socket, string command) {

    return cache->process_command(socket, command);
}

void MemcacheServer::send_output(int socket, string output) {

    output.append(suffix);
    send(socket, output.c_str(), output.size(), 0);
}
