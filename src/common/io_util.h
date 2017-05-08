#ifndef MEMCACHED_CLONE_BRUTE_FORCE_IO_UTIL_H
#define MEMCACHED_CLONE_BRUTE_FORCE_IO_UTIL_H

#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <common/log_util.h>
#define MAX_READ_LEN 1

string read_suffix(int socket, string suffix) {

    string command;
    char buffer[MAX_READ_LEN] = {0};
    int read_len = 0;
    while(!ends_with(command, suffix)) {
        read_len = read(socket, buffer, MAX_READ_LEN);

        if(read_len < 0) {
            break;
        }

        command.append(buffer, read_len);
        //log_info << "read_len " << read_len << endl;
        //log_info << "Bytes read so far " << command.c_str() << endl;
    }

    return command;
}

string read_len(int socket, unsigned long long bytes) {

    string block;
    unsigned long long bytes_read = 0;
    char buffer[MAX_READ_LEN] = {0};
    int read_len = 0;

    while(bytes_read < bytes) {
        read_len = read(socket, buffer, MAX_READ_LEN);

        if(read_len < 0) {
            break;
        }

        block.append(buffer, read_len);
        bytes_read += read_len;
        //log_info << "read_len " << read_len << endl;
        //log_info << "Bytes read so far " << block.c_str() << endl;
    }

    return block;

}
#endif //MEMCACHED_CLONE_BRUTE_FORCE_IO_UTIL_H
