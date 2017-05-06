#ifndef MEMCACHED_CLONE_BRUTE_FORCE_IO_UTIL_H
#define MEMCACHED_CLONE_BRUTE_FORCE_IO_UTIL_H

#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#define MAX_READ_LEN 10

string read_suffix(int socket, string suffix) {

    string command;
    char buffer[MAX_READ_LEN] = {0};

    while(!ends_with(command, suffix)) {
        int read_len = read(socket, buffer, MAX_READ_LEN);
        command.append(buffer, read_len);
    }

    return command;
}

string read_len(int socket, int bytes) {

    string block;
    int bytes_read = 0;
    char buffer[MAX_READ_LEN] = {0};

    while(bytes_read < bytes) {
        int read_len = read(socket, buffer, MAX_READ_LEN);
        block.append(buffer, read_len);
        bytes_read += read_len;
    }

    return block;

}
#endif //MEMCACHED_CLONE_BRUTE_FORCE_IO_UTIL_H
