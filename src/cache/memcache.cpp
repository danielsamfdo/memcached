
unsigned long long Memcache::cas_uniq_counter = 0;

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
            output = process_append(socket, tokens);
            break;
        case OPERATIONS::prepend :
            log_info << "PREPEND METHOD " << endl;
            output = process_prepend(socket, tokens);
            break;
        case OPERATIONS::replace :
            log_info << "REPLACE METHOD " << endl;
            output = process_replace(socket, tokens);
            break;
        case OPERATIONS::cas :
            log_info << "CAS METHOD " << endl;
            output = process_cas(socket, tokens);
            break;
        case OPERATIONS::version :
            log_info << "VERSION METHOD " << endl;
            output = process_version();
            break;
        case OPERATIONS::quit :
            log_info << "QUIT METHOD " << endl;
            process_quit();
            break;
        case OPERATIONS::flush_all :
            log_info << "FLUSH ALL METHOD " << endl;
            process_flush_all();
            break;
        case OPERATIONS::bg_gets :
            log_info << "GETS METHOD " << endl;
            output = process_gets(socket, tokens);
            break;
        case OPERATIONS::incr :
            log_info << "INCR METHOD " << endl;
            output = process_incr(socket, tokens);
            break;
        case OPERATIONS::decr :
            log_info << "DECR METHOD " << endl;
            output = process_decr(socket, tokens);
            break;
        case OPERATIONS::del :
            log_info << "DELETE METHOD " << endl;
            output = process_decr(socket, tokens);
            break;
        default:
            log_info << "NO OTHER METHOD " << endl;
            output = "ERROR";
            break;
    }
    return output;
}

unsigned long long Memcache::get_cas_counter(){
    return cas_uniq_counter++;
}

int Memcache::get_memory(size_t mem_need)
{

    if (capacity-memcache_stats.allocated>=mem_need)
    {
        
        return 1;
    }
    //Take care of expired items if time permits
    else
    {

        return Evict(mem_need);
    }
}

uint64_t Memcache::get_time()
{
    /*
    Returns current timestamp in seconds
    */
    return uint64_t((Time_obj::now()-time_start).count());
}

int Memcache::Evict(size_t mem_need)
{
    log_info << "shit got real3" <<endl;
    return 0;
}

void Memcache::UpdateCache(string key,MemcacheElement *e, uint64_t pt)
{
    log_info << "shit got real1" <<endl;
}

string Memcache::process_set(int socket, vector<string> tokens) {

    /** Sample implementation **/
    string output;
    string key = tokens[0];
    tokens.erase(tokens.begin());
    // if(toke)
    MemcacheElement element;
    store_fill(tokens,&element);
    string block = read_len(socket, element.bytes+2);
    block = block.substr(0,block.size()-2); 
    element.block = (block.c_str());
    if (element.lastaccess == nullptr )log_info << "LRU element" <<endl;

    bool no_reply = tokens.back() == "noreply";

    // Get the memory cleared if cache is full
    size_t mem_need = element.bytes;

    if (get_memory(mem_need))
    {
        cache[key] = element; //update stats!
        log_info << "Stored for key " << key << element.block << endl;
        if(! no_reply) {
            output = "STORED";
        }
        memcache_stats.allocated += mem_need;
        std::cout << typeid(element).name() << "\n*******************";
        UpdateCache(key,&element, get_time());
    }

    return output;
}

string Memcache::process_add(int socket, vector<string> tokens) {
    string output = "";
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    string key = tokens[0];
    tokens.erase(tokens.begin());

    cache_iterator = cache.find(key);
    if ( cache_iterator == cache.end() ){
        log_info << key <<" is the key we try to add " << endl;
        MemcacheElement element;
        store_fill(tokens,&element);
        string block = read_len(socket, element.bytes+2);
        block = block.substr(0,block.size()-2); 
        element.block = (block.c_str());

        bool no_reply = tokens.back() == "noreply";

        cache[key] = element; //update stats!
        log_info << "Stored for key " << key << element.block << endl;
        if(! no_reply) {
            output = "STORED";
        }

        return output;
    }
    else{
        // NO ACTION SHOULD BE DONE
    }

    return output;
}

string Memcache::response_get(string key, MemcacheElement elt){
    return "VALUE " + key + " " + to_string(elt.flags) + " " + to_string(elt.bytes) + "\r\n";
}

void Memcache::update_store_fill(MemcacheElement *element,vector<string> tokens){
    log_info << " FLAGS " << str_cast<uint16_t>(tokens[0]) << endl;
    element->flags = str_cast<uint16_t>(tokens[0]);
    log_info << " EXP TIME " << str_cast<int>(tokens[1]) << endl;
    element->exptime = str_cast<uint64_t>(tokens[1]);
    element->bytes += str_cast<size_t>(tokens[2]);
    log_info << " BYTES " << element->bytes << endl;
    element->cas_unique = Memcache::get_cas_counter();
}

string Memcache::process_append(int socket, vector<string> tokens) {
    string output = "";
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    string key = tokens[0];
    tokens.erase(tokens.begin());

    cache_iterator = cache.find(key);
    if ( cache_iterator == cache.end() ){
        // NO ACTION SHOULD BE DONE
    }
    else{
        MemcacheElement element = cache_iterator->second;
        log_info <<key<<" is the key we try to append" << endl;
        update_store_fill(&element, tokens);
        string block = read_len(socket, str_cast<int>(tokens[2])+2);
        block = block.substr(0,block.size()-2); 
        element.block += (block.c_str());
        bool no_reply = tokens.back() == "noreply";
        if(! no_reply) {
            output = "STORED";
        }
        cache[key] = element; //update stats!
        log_info << "Stored for key " << key << element.block << endl;
        
        return output;
    }
    return output;
}

string Memcache::process_prepend(int socket, vector<string> tokens) {
    string output = "";
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    string key = tokens[0];
    tokens.erase(tokens.begin());

    cache_iterator = cache.find(key);
    if ( cache_iterator == cache.end() ){
        // NO ACTION SHOULD BE DONE
    }
    else{
        MemcacheElement element = cache_iterator->second;
        log_info <<key<<" is the key we try to prepend" << endl;
        update_store_fill(&element, tokens);
        string block = read_len(socket, str_cast<int>(tokens[2])+2);
        block = block.substr(0,block.size()-2); 
        element.block = (block.c_str()) + element.block;

        bool no_reply = tokens.back() == "noreply";

        cache[key] = element; //update stats!
        log_info << "Stored for key " << key << element.block << endl;
        if(! no_reply) {
            output = "STORED";
        }
        return output;
    }
    return output;
}

string Memcache::process_replace(int socket, vector<string> tokens) {
    string output = "";
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    string key = tokens[0];
    tokens.erase(tokens.begin());

    cache_iterator = cache.find(key);
    if ( cache_iterator == cache.end() ){
        // NO ACTION SHOULD BE DONE
    }
    else{
        log_info <<key<<" is the key we try to replace" << endl;
        MemcacheElement element;
        store_fill(tokens,&element);
        string block = read_len(socket, element.bytes+2);
        block = block.substr(0,block.size()-2); 
        element.block = (block.c_str());

        bool no_reply = tokens.back() == "noreply";

        cache[key] = element; //update stats!
        log_info << "Stored for key " << key << element.block << endl;
        if(! no_reply) {
            output = "STORED";
        }
        return output;
    }

    return output;
}

string Memcache::process_cas(int socket, vector<string> tokens) {
    string output = "";
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    string key = tokens[0];
    tokens.erase(tokens.begin());

    cache_iterator = cache.find(key);
    if ( cache_iterator == cache.end() ){
        output = "NOT_FOUND";
        // NO ACTION SHOULD BE DONE
    }
    else{
        log_info <<key<<" is the key we try to CAS" << endl;
        MemcacheElement element;
        store_fill(tokens,&element);
        string block = read_len(socket, element.bytes+2);
        block = block.substr(0,block.size()-2); 
        element.block = (block.c_str());

        bool no_reply = tokens.back() == "noreply";

        string::size_type sz = 0; 
        unsigned long long cas_uniq = stoull (tokens[3],&sz,0);
        if(cache[key].cas_unique == cas_uniq){
            output = "STORED";
            cache[key] = element; //update stats!
            log_info << "CAS Stored for key " << key << element.block << endl;
        } else {
            output = "EXISTS";
            log_info << "CAS Not Stored for key since cas number not unique " << key << element.block << endl;
        }
        if(no_reply) {
            output = "";
        }
        
        return output;
    }

    return output;
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
    // return "yo";
}

void Memcache::store_fill(vector<string> tokens, MemcacheElement *element) {
    // MemcacheElement element;
    log_info << " FLAGS " << str_cast<uint16_t>(tokens[0]) << endl;
    element->flags = str_cast<uint16_t>(tokens[0]);
    log_info << " EXP TIME " << str_cast<int>(tokens[1]) << endl;
    element->exptime = str_cast<uint64_t>(tokens[1]);
    log_info << " BYTES " << str_cast<size_t>(tokens[2]) << endl;
    element->bytes = str_cast<int>(tokens[2]);
    element->cas_unique = Memcache::get_cas_counter();
    // return element;
}

string Memcache::process_version(){
    return "1.5.8";
}

void Memcache::process_quit(){
    return;
    // return "1.5.8";
}

void Memcache::process_flush_all(){
    cache.clear();
    return ;
}

string Memcache::process_delete(int socket, vector<string> tokens){
    string output = "";
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    MemcacheElement res;
    string key = tokens[0];

    cache_iterator = cache.find(key);
    if ( cache_iterator == cache.end() ){
        output = "NOT_FOUND";
    }
    else{
        res = cache_iterator->second;
        log_info << "Present in CACHE" << endl;
        cache.erase(key); 
        output = "DELETED";
    }
    log_info << output << endl;
    bool no_reply = tokens.back() == "noreply";
    if(no_reply){
        output = "";
    }
    return output;
}

string Memcache::process_incr(int socket, vector<string> keys){
    return "";
}

string Memcache::process_decr(int socket, vector<string> keys){
    return "";
}

string Memcache::process_gets(int socket, vector<string> keys){
    return "";
}
