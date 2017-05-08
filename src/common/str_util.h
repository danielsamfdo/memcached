#ifndef MEMCACHED_CLONE_BRUTE_FORCE_STR_UTIL_H
#define MEMCACHED_CLONE_BRUTE_FORCE_STR_UTIL_H

#include <regex>

using namespace std;

//Ref - http://stackoverflow.com/a/20446239/1026535
bool ends_with(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

vector<string> tokenize(string command) {
    vector<string> tokens;

    regex re("\\s+");
    sregex_token_iterator it(command.begin(), command.end(), re, -1);
    sregex_token_iterator end;

    for(; it != end; ++it) {
        tokens.push_back(it->str());
    }

    return tokens;
}

template <typename T>
T str_cast(string str) {

    T var;
    istringstream buffer(str);
    buffer >> var;

    return var;
}

// http://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

#endif //MEMCACHED_CLONE_BRUTE_FORCE_STR_UTIL_H