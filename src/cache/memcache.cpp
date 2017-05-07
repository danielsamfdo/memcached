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
        case OPERATIONS::append :
            log_info << "APPEND METHOD " << endl;
            output = process_add(socket, tokens);
            break;
        case OPERATIONS::prepend :
            log_info << "PREPEND METHOD " << endl;
            output = process_add(socket, tokens);
            break;
        case OPERATIONS::replace :
            log_info << "REPLACE METHOD " << endl;
            output = process_add(socket, tokens);
            break;
        case OPERATIONS::cas :
            log_info << "CAS METHOD " << endl;
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
    string block = read_len(socket, element.bytes);
    element.block = (block.c_str());

    bool no_reply = tokens.back() == "noreply";

    cache[key] = element; //update stats!
    log_info << "Stored for key " << key << element.block << endl;
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
    return "VALUE " + key + " " + to_string(elt.flags) + " " + to_string(elt.bytes)  + "\r\n";
}

string Memcache::process_append(int socket, vector<string> keys) {
    return "";
}

string Memcache::process_prepend(int socket, vector<string> keys) {
    return "";
}

string Memcache::process_replace(int socket, vector<string> keys) {
    return "";
}

string Memcache::process_cas(int socket, vector<string> keys) {
    return "";
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
            string response = response_get(keys[it], res);
            log_info << "Present in CACHE" << endl;
            output += response;
            output += res.block;
            output += "\r\n"; 
            // string str((char *)res.block);
            // output += str + "\r\n";
        }
    }
    output += "END";
    log_info << output << endl;

    return output;
}

MemcacheElement Memcache::store_fill(vector<string> tokens) {
    MemcacheElement element;
    log_info << " FLAGS " << str_cast<uint16_t>(tokens[0]) << endl;
    element.flags = str_cast<uint16_t>(tokens[0]);
    log_info << " EXP TIME " << str_cast<int>(tokens[1]) << endl;
    element.exptime = str_cast<int>(tokens[1]);
    log_info << " BYTES " << str_cast<int>(tokens[2]) << endl;
    element.bytes = str_cast<int>(tokens[2]);

    return element;
}