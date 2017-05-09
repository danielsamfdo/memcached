unsigned long long Memcache::cas_uniq_counter = 0;

void Memcache::check_and_evict(string key){
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    cache_iterator = cache.find(key);
    if(cache_iterator!=cache.end()){
        MemcacheElement *elt = &cache_iterator->second;
        log_info << " TIME :  " << get_current_time_in_seconds() << " EXPIRED TIME : " << elt->exptime << "   " << elt->expired(get_current_time_in_seconds()) << endl; 
        if(elt->expired(get_current_time_in_seconds())){

            Clear_CacheElement(key);
            memcache_stats.allocated-=elt->bytes;
            cache.erase(key);
        }
    }
    return ;
}

void Memcache::update_store_fill(MemcacheElement *element,vector<string> tokens, bool just_bytes){
    log_info << " FLAGS " << str_cast<uint16_t>(tokens[0]) << endl;
    element->flags = str_cast<uint16_t>(tokens[0]);
    element->update_expiration_time(get_current_time_in_seconds() ,str_cast<uint64_t>(tokens[1]));
    log_info << " EXP TIME " << element->exptime << endl;
    if(!just_bytes)
        element->bytes += str_cast<uint64_t>(tokens[2]);
    else
        element->bytes = str_cast<uint64_t>(tokens[2]);
    log_info << " BYTES " << element->bytes << endl;
    element->cas_unique = Memcache::get_cas_counter();
}

string Memcache::process_command(int socket, string command) {
    log_info << "Time : " << to_string(get_current_time_in_seconds()) << " Processing command " << command.c_str() << endl;
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
            process_quit(socket);
            break;
        case OPERATIONS::flush_all :
            log_info << "FLUSH ALL METHOD " << endl;
            output = process_flush_all();
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
            output = process_delete(socket, tokens);
            break;
        case OPERATIONS::stats :
            log_info << "STATS METHOD " << endl;
            output = process_stats();
            break;
        default:
            log_info << "NO OTHER METHOD " << endl;
            output = "ERROR";
            break;
    }
    
    return output;
}

string Memcache::process_stats(){
    memcache_stats.curr_items = cache.size();
    return memcache_stats.process_get_statistics(get_current_time_in_seconds(),process_version());
}

unsigned long long Memcache::get_cas_counter(){
    return cas_uniq_counter++;
}


int Memcache::get_memory(uint64_t mem_need)
{

    if (capacity-memcache_stats.allocated>=mem_need && memcache_stats.allocated <=capacity)
    {
        
        return 1;
    }
    //Take care of expired items if time permits
    else
    {

        return Evict(mem_need);
    }
}

uint64_t Memcache::get_current_time_in_seconds(){
    long long ten_power_nine = 1000000000;
    time_p p = Time_obj::now();
    return uint64_t((p-time_start).count())/ten_power_nine;
}

uint64_t Memcache::get_time()
{
    /*
    Returns current timestamp in seconds
    */
    time_p p = Time_obj::now();
    return uint64_t((p-time_start).count());
}

int Memcache::Evict(uint64_t mem_need)
{
    log_info << "shit got real3" <<endl;
    return 0;
}

void Memcache::UpdateCache(string key, uint64_t pt)
{
    log_info << "shit got real1" <<endl;
}

void Memcache::Clear_CacheElement(string key)
{

}
void Memcache::Clear_CacheAll()
{

}

void Memcache::Cache_miss(string key, uint64_t pt){}
void Memcache::lockAll(){
    log_info<<"In lockall"<<endl;
    for(int i=0;i<NLOCKS;i++)
    {
        // char c = i;
        // log_info<< c <<endl;
        Mutexvariables[i].lock();
    }
    log_info << "Locking All Keys" << endl;
    return ;
}

void Memcache::unlockAll(){
    log_info << "Unlocking ALL KEYS" << endl;
    for(int i=0;i<NLOCKS;i++)
        Mutexvariables[i].unlock();
    return ;
}

void Memcache::lock(char key){
    Mutexvariables[key].lock();
    log_info << "Locking " << key << endl;
    return ;
}

void Memcache::unlock(char key){
    log_info << "Unlocking " << key << endl;
    Mutexvariables[key].unlock();
    return ;

}

string key_size_check(string key){
    int max_key_size = 250;
    log_info << "CHECKING KEYS SIZE :: " << key.length() << endl;
    if(key.length() > max_key_size)
        return "CLIENT_ERROR length is > " + to_string(max_key_size);
    return "OK";
}

string Memcache::valid_format_storage_commands(vector<string> tokens, bool cas) {
    if(!cas){
        for(int i=0;i< tokens.size();i++)
            log_info << tokens[i] << endl;

        log_info << "KEY FORMAT CHECK " << to_string(tokens.size()) << (tokens.size()==4 || (tokens.size() == 5 && tokens.back()=="noreply")) << endl;
        if((tokens.size()==4 || (tokens.size() == 5 && tokens.back()=="noreply"))){

            if(!is_number(tokens[1]) || !is_number(tokens[2]) || !is_number(tokens[3]))
                return "CLIENT_ERROR Format of command is invalid";
            log_info << (max_val_bytes_possible < str_cast<uint64_t>(tokens[3])) << " is checked asked for : " << str_cast<uint64_t>(tokens[3]) <<  " with max cap : " << max_val_bytes_possible << endl;
            if(max_val_bytes_possible < str_cast<uint64_t>(tokens[3])){
                return "CLIENT_ERROR MAX VALUE THAT CAN BE STORED IS " + to_string(max_val_bytes_possible);
            }
        }
        else{
            return "CLIENT_ERROR Format of command is invalid";
        }
    }
    if(cas){
        if((tokens.size()==5 || (tokens.size() == 6 && tokens.back()=="noreply"))){
            if(!is_number(tokens[1]) || !is_number(tokens[2]) || !is_number(tokens[3]) || !is_number(tokens[4]) )
                return "CLIENT_ERROR Format of command is invalid";
            log_info << (max_val_bytes_possible < str_cast<uint64_t>(tokens[3])) << " is checked asked for : " << str_cast<uint64_t>(tokens[3]) <<  " with max cap : " << max_val_bytes_possible << endl;
            if(max_val_bytes_possible < str_cast<uint64_t>(tokens[3])){
                return "CLIENT_ERROR MAX VALUE THAT CAN BE STORED IS " + to_string(max_val_bytes_possible);
            }
        }
        else{
            return "CLIENT_ERROR Format of command is invalid";
        }
    }
    string response = key_size_check(tokens[0]);
    if(response != "OK"){
        return response;
    }

    return "OK";
}

void Memcache::Operations(string key){
    check_and_evict(key);
    return ;
}


string Memcache::process_set(int socket, vector<string> tokens) {

    /** Sample implementation **/
    memcache_stats.cmd_set++;
    string output;
    string response = valid_format_storage_commands(tokens);
    if(response != "OK"){
        log_info << response;
        return response;
    }
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    string key = tokens[0];
    Operations(key);
    Memcache::lock(key[0]);
    cache_iterator = cache.find(key);
    // log_info << "Locking " << key[0] << endl;
    tokens.erase(tokens.begin());
    MemcacheElement element;
    bool flag = true;
    uint64_t tmpsize = 0;
    if ( cache_iterator != cache.end() ){
        element = cache_iterator->second;
        flag = false;
        tmpsize = element.bytes;
    }
    update_store_fill(&element, tokens, true);

    string block = read_len(socket, element.bytes+2);
    block = block.substr(0,block.size()-2); 
    element.block = (block.c_str());
    bool no_reply = tokens.back() == "noreply";

    // if (element.lastaccess == nullptr )log_info << "LRU element" <<endl;

    // Get the memory cleared if cache is full
    uint64_t mem_need = element.bytes;
    if (flag)
    {
        // Unlocking the present locked key so that evict can lock all keys
        Memcache::unlock(key[0]);   
        if (!get_memory(mem_need))
        {
            output = "SERVER_ERROR Memory";
            log_info << "Couldn't recover memory" << endl;
            return output;
        }
        //locking back the key again before saving
        Memcache::lock(key[0]);
        memcache_stats.allocated += mem_need;
        cache_iterator = cache.find(key);
        if ( cache_iterator != cache.end() )
        memcache_stats.allocated -= tmpsize;
    }
    cache[key] = element; //update stats!
    log_info << "Stored for key " << key << element.block << endl;
    if(! no_reply) {
        output = "STORED";
    }
    
    // std::cout << typeid(element).name() << "\n*******************";
    UpdateCache(key, get_time()); 
    // log_info << "Unlocking " << key[0] << endl;
    Memcache::unlock(key[0]);
    return output;
}

string Memcache::process_add(int socket, vector<string> tokens) {
    string output = "";
    string response = valid_format_storage_commands(tokens);
    if(response != "OK"){
        log_info << response;
        return response;
    }
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    string key = tokens[0];
    Operations(key);
    Memcache::lock(key[0]);

    tokens.erase(tokens.begin());

    cache_iterator = cache.find(key);
    if ( cache_iterator == cache.end() ){

        log_info << key <<" is the key we try to add " << endl;
        MemcacheElement element;
        update_store_fill(&element,tokens,true);
        string block = read_len(socket, element.bytes+2);
        block = block.substr(0,block.size()-2); 
        element.block = (block.c_str());

        bool no_reply = tokens.back() == "noreply";

        // Checking for memory in our cache
        uint64_t mem_need = element.bytes;
        Memcache::unlock(key[0]);   
        if (!get_memory(mem_need))
        {
            output = "SERVER_ERROR Memory";
            log_info << "Couldn't recover memory" << endl;
            return output;
        }
        //locking back the key again before saving
        Memcache::lock(key[0]);
        memcache_stats.allocated += mem_need;

        cache[key] = element; //update stats!
        UpdateCache(key, get_time());
        log_info << "Stored for key " << key << element.block << endl;
        if(! no_reply) {
            output = "STORED";
        }
    }
    else{
        output = "CLIENT_ERROR Key already present Cant be added";
        // NO ACTION SHOULD BE DONE
    }
    Memcache::unlock(key[0]);
    return output;
}

string Memcache::response_get(string key, MemcacheElement elt, bool gets){
    string cas = "";
    if(gets)
        cas = " " + to_string(elt.cas_unique);
    return "VALUE " + key + " " + to_string(elt.flags) + " " + to_string(elt.bytes) + cas + "\r\n";
}


string Memcache::process_append(int socket, vector<string> tokens) {
    string output = "";
    string response = valid_format_storage_commands(tokens);
    if(response != "OK"){
        log_info << response;
        return response;
    }
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    string key = tokens[0];
    Operations(key);
    Memcache::lock(key[0]);

    tokens.erase(tokens.begin());

    cache_iterator = cache.find(key);
    if ( cache_iterator == cache.end() ){
        output = "CLIENT_ERROR Key not present Cant be fed";
        // NO ACTION SHOULD BE DONE
    }
    else{
        MemcacheElement element = cache_iterator->second;
        uint64_t tmpsize = element.bytes;
        log_info <<key<<" is the key we try to append" << endl;
        update_store_fill(&element, tokens);
        string block = read_len(socket, str_cast<uint64_t>(tokens[2])+2);
        log_info << "----------------------------------" <<block << endl;
        block = block.substr(0,block.size()-2); 
        element.block += (block.c_str());
        log_info << "----------------------------------" <<block << endl;
        bool no_reply = tokens.back() == "noreply";
        if(! no_reply) {
            output = "STORED";
        }
        uint64_t mem_need = element.bytes;
        Memcache::unlock(key[0]);   
        if (!get_memory(mem_need))
        {
            output = "SERVER_ERROR Memory";
            log_info << "Couldn't recover memory" << endl;
            return output;
        }
        //locking back the key again before saving
        Memcache::lock(key[0]);
        memcache_stats.allocated += mem_need;
        cache_iterator = cache.find(key);
        if ( cache_iterator != cache.end() )
        memcache_stats.allocated -= tmpsize;
        
        cache[key] = element; //update stats!
        UpdateCache(key, get_time());
        log_info << "Stored for key " << key << element.block << endl;
        
    }
    Memcache::unlock(key[0]);
    return output;
}
/**
 * [Memcache::process_prepend description]
 * @param  socket [description]
 * @param  tokens [description]
 * @return        [description]
 */
string Memcache::process_prepend(int socket, vector<string> tokens) {
    string output = "";
    string response = valid_format_storage_commands(tokens);
    if(response != "OK"){
        log_info << response;
        return response;
    }
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    string key = tokens[0];
    Operations(key);
    Memcache::lock(key[0]);
    tokens.erase(tokens.begin());

    cache_iterator = cache.find(key);
    if ( cache_iterator == cache.end() ){
        output = "CLIENT_ERROR Key not present Cant be fed";
    }
    else{
        MemcacheElement element = cache_iterator->second;
        uint64_t tmpsize = element.bytes;
        log_info <<key<<" is the key we try to prepend" << endl;
        update_store_fill(&element, tokens);
        string block = read_len(socket, str_cast<uint64_t>(tokens[2])+2);
        block = block.substr(0,block.size()-2); 
        element.block = (block.c_str()) + element.block;

        bool no_reply = tokens.back() == "noreply";

        uint64_t mem_need = element.bytes;
        Memcache::unlock(key[0]);   
        if (!get_memory(mem_need))
        {
            output = "SERVER_ERROR Memory";
            log_info << "Couldn't recover memory" << endl;
            return output;
        }
        //locking back the key again before saving
        Memcache::lock(key[0]);
        memcache_stats.allocated += mem_need;
        cache_iterator = cache.find(key);
        if ( cache_iterator != cache.end() )
        memcache_stats.allocated += mem_need;
        cache_iterator = cache.find(key);
        if ( cache_iterator != cache.end() )
        memcache_stats.allocated -= tmpsize;

        cache[key] = element; //update stats!
        UpdateCache(key, get_time());
        log_info << "Stored for key " << key << element.block << endl;
        if(! no_reply) {
            output = "STORED";
        }
    }
    Memcache::unlock(key[0]);
    return output;
}

string Memcache::process_replace(int socket, vector<string> tokens) {
    string output = "";
    string response = valid_format_storage_commands(tokens);
    if(response != "OK"){
        log_info << response;
        return response;
    }
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    string key = tokens[0];
    Operations(key);
    Memcache::lock(key[0]);
    tokens.erase(tokens.begin());

    cache_iterator = cache.find(key);
    if ( cache_iterator == cache.end() ){
        // NO ACTION SHOULD BE DONE
    }
    else{
        log_info <<key<<" is the key we try to replace" << endl;
        MemcacheElement element = cache_iterator->second;
        uint64_t tmpsize = element.bytes;
        update_store_fill(&element,tokens,true);
        string block = read_len(socket, element.bytes+2);
        block = block.substr(0,block.size()-2); 
        element.block = (block.c_str());

        bool no_reply = tokens.back() == "noreply";

        uint64_t mem_need = element.bytes;
        Memcache::unlock(key[0]);   
        if (!get_memory(mem_need))
        {
            output = "SERVER_ERROR Memory";
            log_info << "Couldn't recover memory" << endl;
            return output;
        }
        //locking back the key again before saving
        Memcache::lock(key[0]);
        memcache_stats.allocated += mem_need;
        cache_iterator = cache.find(key);
        if ( cache_iterator != cache.end() )
        memcache_stats.allocated -= tmpsize;

        cache[key] = element; //update stats!
        UpdateCache(key, get_time());

        log_info << "Stored for key " << key << element.block << endl;
        if(! no_reply) {
            output = "STORED";
        }
    }
    Memcache::unlock(key[0]);
    return output;
}

string Memcache::process_cas(int socket, vector<string> tokens) {
    string output = "";
    string response = valid_format_storage_commands(tokens, true);
    if(response != "OK"){
        log_info << response;
        return response;
    }
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    string key = tokens[0];
    Operations(key);
    Memcache::lock(key[0]);
    tokens.erase(tokens.begin());

    cache_iterator = cache.find(key);
    if ( cache_iterator == cache.end() ){
        output = "NOT_FOUND";
        memcache_stats.cas_misses++;
        // NO ACTION SHOULD BE DONE
    }
    else{
        memcache_stats.cas_hits++;
        log_info <<key<<" is the key we try to CAS" << endl;
        MemcacheElement element = cache_iterator->second;
        uint64_t tmpsize = element.bytes;
        update_store_fill(&element,tokens,true);
        string block = read_len(socket, element.bytes+2);
        block = block.substr(0,block.size()-2); 
        element.block = (block.c_str());

        bool no_reply = tokens.back() == "noreply";

        string::size_type sz = 0; 
        unsigned long long cas_uniq = stoull (tokens[3],&sz,0);
        if(cache[key].cas_unique == cas_uniq){
            output = "STORED";
            uint64_t mem_need = element.bytes;
            Memcache::unlock(key[0]);   
            if (!get_memory(mem_need))
            {
                output = "SERVER_ERROR Memory";
                log_info << "Couldn't recover memory" << endl;
                return output;
            }
            //locking back the key again before saving
            Memcache::lock(key[0]);
            memcache_stats.allocated += mem_need;
            cache_iterator = cache.find(key);
            if ( cache_iterator != cache.end() )
            memcache_stats.allocated -= tmpsize;
            cache[key] = element; //update stats!
            UpdateCache(key, get_time()); //update stats!
            log_info << "CAS Stored for key " << key << element.block << endl;
        } else {
            output = "EXISTS";
            log_info << "CAS Not Stored for key since cas number not unique " << key << element.block << endl;
        }
        if(no_reply) {
            output = "";
        }
        
    }
    Memcache::unlock(key[0]);
    return output;
}


string Memcache::process_get(int socket, vector<string> keys, bool gets) {
    string output = "";
    memcache_stats.cmd_get++;
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    MemcacheElement* res;
    for(int it=0;it<keys.size();it++){
        check_and_evict(keys[it]);
    }
    for(int it=0;it<keys.size();it++){
        string key = keys[it];
        Memcache::lock(key[0]);
        cache_iterator = cache.find(key);

        if ( cache_iterator == cache.end() ){
            // Get Misses
            memcache_stats.get_misses++;
            Cache_miss(key,get_time());
        }
        else{
            memcache_stats.get_hits++;

            res = &cache_iterator->second;
            string response = response_get(keys[it], *res, gets);
            log_info << "Present in CACHE" << endl;
            output += response;
            output += res->block;
            output += "\r\n"; 
            UpdateCache(key, get_time());
            // string str((char *)res.block);
            // output += str + "\r\n";
        }
        Memcache::unlock(key[0]);
    }
    output += "END";
    log_info << output << endl;

    return output;
    // return "yo";
}

string Memcache::process_version(){
    return "1.5.8";
}

void Memcache::process_quit(int socket){
    shutdown(socket, SHUT_RDWR);
    close(socket);
    //terminate();
    //exit(0);
    return ;
    // return "1.5.8";
}

string Memcache::process_flush_all(){
    memcache_stats.cmd_flush++;
    Memcache::lockAll();
    Clear_CacheAll();
    cache.clear();
    memcache_stats.allocated = 0;
    Memcache::unlockAll();
    return "OK";
}

string Memcache::process_delete(int socket, vector<string> tokens){
    string output = "";
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    MemcacheElement res;
    string key = tokens[0];
    Operations(key);
    Memcache::lock(key[0]);
    try {
        cache_iterator = cache.find(key);
        if ( cache_iterator == cache.end() ){
            memcache_stats.delete_misses++;
            output = "NOT_FOUND";
        }
        else{
            memcache_stats.delete_hits++;
            res = cache_iterator->second;
            log_info << "Present in CACHE" << endl;
            Clear_CacheElement(key);
            memcache_stats.allocated -= res.bytes;
            cache.erase(key); 
            output = "DELETED";
        }
        log_info << output << endl;
        bool no_reply = tokens.back() == "noreply";
        if(no_reply){
            output = "";
        }
    }
    catch (exception& e)
    {
        log_error << e.what() << endl;
    }
    Memcache::unlock(key[0]);
    return output;
}

string Memcache::process_incr(int socket, vector<string> tokens){
    string output = "";
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    MemcacheElement res;
    string key = tokens[0];
    Operations(key);
    Memcache::lock(key[0]);
    try {
        cache_iterator = cache.find(key);
        if ( cache_iterator == cache.end() ){
            output = "NOT_FOUND";
            memcache_stats.incr_misses++;
        }
        else{
            memcache_stats.incr_hits++;
            res = cache_iterator->second;
            log_info << "Present in CACHE" << endl;
            if(!is_number(res.block) || !is_number(tokens[1]))
                output = "CLIENT_ERROR Cache value or incr val tokens by client is not a number";
            else{
                uint64_t tmpsize = res.bytes;
                res.block = to_string(str_cast<uint64_t>(res.block) + str_cast<uint64_t>(tokens[1]));
                output = res.block;
                res.bytes = res.block.size();
                uint64_t mem_need = res.bytes;
                Memcache::unlock(key[0]);   
                if (!get_memory(mem_need))
                {
                    output = "SERVER_ERROR Memory";
                    log_info << "Couldn't recover memory" << endl;
                    return output;
                }
                //locking back the key again before saving
                Memcache::lock(key[0]);
                memcache_stats.allocated += mem_need;
                cache_iterator = cache.find(key);
                if ( cache_iterator != cache.end() )
                memcache_stats.allocated -= tmpsize;
                cache[key] = res;
                UpdateCache(key, get_time()); //update stats!
            }
        }
        log_info << output << endl;
        bool no_reply = tokens.back() == "noreply";
        if(no_reply){
            output = "";
        }
    }
    catch (exception& e)
    {
        log_error << e.what() << endl;
    }
    Memcache::unlock(key[0]);
    return output;
}

string Memcache::process_decr(int socket, vector<string> tokens){
    string output = "";
    unordered_map<string, MemcacheElement>::iterator cache_iterator;
    MemcacheElement res;
    string key = tokens[0];
    Operations(key);
    Memcache::lock(key[0]);
    try {
        cache_iterator = cache.find(key);
        if ( cache_iterator == cache.end() ){
            memcache_stats.decr_misses++;
            output = "NOT_FOUND";
        }
        else{
            memcache_stats.decr_hits++;
            res = cache_iterator->second;
            log_info << "Present in CACHE" << str_cast<uint64_t>(res.block) << endl;
            if(!is_number(res.block) || !is_number(tokens[1]))
                output = "CLIENT_ERROR Cache value or decr val tokens by client is not a number";
            else{
                uint64_t tmpsize = res.bytes;
                uint64_t v1 = str_cast<uint64_t>(res.block);
                uint64_t v2 = str_cast<uint64_t>(tokens[1]);
                if(v1 < v2)
                    res.block = to_string(0);
                else
                    res.block = to_string(v1-v2);
                output = res.block;
                res.bytes = res.block.size();
                uint64_t mem_need = res.bytes;
                Memcache::unlock(key[0]);   
                if (!get_memory(mem_need))
                {
                    output = "SERVER_ERROR Memory";
                    log_info << "Couldn't recover memory" << endl;
                    return output;
                }
                //locking back the key again before saving
                Memcache::lock(key[0]);
                memcache_stats.allocated += mem_need;
                cache_iterator = cache.find(key);
                if ( cache_iterator != cache.end() )
                memcache_stats.allocated -= tmpsize;
                cache[key] = res;
                UpdateCache(key, get_time()); //update stats!
            }
        }
        log_info << output << endl;
        bool no_reply = tokens.back() == "noreply";
        if(no_reply){
            output = "";
        }
    }
    catch (exception& e)
    {
        log_error << e.what() << endl;
    }
    Memcache::unlock(key[0]);
    return output;
}

string Memcache::process_gets(int socket, vector<string> keys){

    return process_get(socket, keys, true);
}
