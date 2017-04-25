MemcacheServer::MemcacheServer(int cache_type, int port) : TCPServer(port) {
    init(cache_type);
}

MemcacheServer::MemcacheServer(int cache_type, int port, int max_conn_backlog, int max_read_len) : TCPServer(port, max_conn_backlog) {
    init(cache_type);
}

void MemcacheServer::init(int cache_type) {
    max_read_len = 10;
    suffix = "\r\n";

    switch(cache_type) {
        case 1:
            cache = new LRUCache();
            break;
        case 2:
            cache = new RandomReplacementCache();
            break;
        case 3:
            cache = new LandlordCache();
    }
}

void MemcacheServer::process_conn(int socket) {

    while(true) {
        string command = read_command(socket);
        string output = process_command(socket);

        if(! output.empty()) {
            send_output(socket, output);
        }
    }
}

string read_command(int socket) {

    string command;
    char buffer[max_read_len] = {0};

    while(!ends_with(command, suffix)) {
        int read_len = read(socket, buffer, max_read_len);
        command.append(buffer, read_len);
    }

    return command;
}

string process_command(int socket) {

    return cache.process_command(socket);
}

void send_output(int socket, string output) {

    output.append(suffix);
    send(socket, output.c_str(), output.size(), 0);
}
