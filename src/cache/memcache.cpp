string Memcache::process_command(int socket, string command) {
    log_info << "Processing command " << command.c_str() << endl;

    vector<string> tokens = tokenize(command);
    string command_type = tokens[0], output;

    tokens.erase(tokens.begin()); //remove first token

    if(command_type == "set" ) {
        output = process_set(socket, tokens);
    }
    else if(command_type == "add") {
        output = process_add(socket, tokens);
    }
    else {
        output = "Hello";
    }
    return output;
}

string Memcache::process_set(int socket, vector<string> tokens) {

    /** Sample implementation **/

    string key = tokens[0], output;
    tokens.erase(tokens.begin());

    MemcacheElement element = store_fill(tokens);
    bool no_reply = tokens.back() == "noreply";

    cache[key] = element; //update stats!

    if(! no_reply) {
        output = "STORED";
    }

    return output;
}

string Memcache::process_add(int socket, vector<string> tokens) {
    string output = "";
    return output;
}

string Memcache::process_get(int socket, vector<string> tokens) {
    string output = "";
    return output;
}

MemcacheElement Memcache::store_fill(vector<string> tokens) {
    MemcacheElement element;

    element.flags = str_cast<uint16_t>(tokens[0]);
    element.exptime = str_cast<int>(tokens[1]);
    element.bytes = str_cast<int>(tokens[2]);

    return element;
}