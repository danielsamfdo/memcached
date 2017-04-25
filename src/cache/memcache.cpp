string Memcache::process_command(int socket, string command) {

    vector<string> tokens = tokenize(command);
    string command_type = tokens[0], output;

    tokens.erase(tokens.begin()); //remove first token

    if(command_type == "set" ) {
        output = process_set(socket, tokens);
    }
    else if(command_type == "add") {
        output = process_add(socket, tokens);
    }

    return output;
}

string Memcache::process_set(int socket, vector<string> tokens) {

}

string Memcache::process_add(int socket, vector<string> tokens) {

}