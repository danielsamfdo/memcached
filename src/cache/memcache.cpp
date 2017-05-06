string Memcache::process_command(int socket, string command) {
    log_info << "Processing command " << command.c_str() << endl;
    log_info << "Hi" << endl;
    vector<string> tokens = tokenize(command);
    for(int i=0;i<tokens.size();i++){
        log_info << tokens[i] << endl;
    }
    string output;
    int command_type = this->operation[tokens[0]];
    log_info << command_type << endl;
    tokens.erase(tokens.begin()); //remove first token
    switch(command_type){
        case OPERATIONS::set :
            log_info << "SET METHOD" << endl;
            output = process_set(socket, tokens);
            break;
        case OPERATIONS::get :
            log_info << "GET METHOD " << endl;
            output = process_get(socket, tokens);
            break;
        case OPERATIONS::add :
            log_info << "ADD METHOD " << endl;
            output = process_add(socket, tokens);
            break;
    }
    return output;
}

string Memcache::process_set(int socket, vector<string> tokens) {

    /** Sample implementation **/
    string output;
    string key = tokens[0];
    tokens.erase(tokens.begin());

    MemcacheElement element = store_fill(tokens);
    bool no_reply = tokens.back() == "noreply";

    cache[key] = element; //update stats!
    log_info << "Stored for key " << key << "element" << endl;
    if(! no_reply) {
        output = "STORED";
    }

    return output;
}

string Memcache::process_add(int socket, vector<string> tokens) {

    string output = "";
    return output;
}

string response_get(string key, MemcacheElement elt){
    return "test";
    // return "VALUE " + key + " " + elt.flags + " " + elt.bytes + " " + elt.cas_unique + "\r\n"
}

string Memcache::process_get(int socket, vector<string> keys) {
    string output = "";
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    MemcacheElement res;
    for(int it=0;it<keys.size();it++){
        cache_iterator = cache.find(keys[it]);
        if ( cache_iterator == cache.end() ){

        }
        else{
            res = cache_iterator->second;
            log_info << "Present in CACHE" << endl;
            // output += response_get(keys[it],res);
        }
    }
    return output;
}

MemcacheElement Memcache::store_fill(vector<string> tokens) {
    MemcacheElement element;

    element.flags = str_cast<uint16_t>(tokens[0]);
    element.exptime = str_cast<int>(tokens[1]);
    element.bytes = str_cast<int>(tokens[2]);

    return element;
}