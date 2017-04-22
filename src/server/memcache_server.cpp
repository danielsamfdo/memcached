MemcacheServer::MemcacheServer(int port) : TCPServer(port) {

}

MemcacheServer::MemcacheServer(int port, int max_conn_backlog) : TCPServer(port, max_conn_backlog) {

}

void MemcacheServer::process_conn(int socket) {

}
