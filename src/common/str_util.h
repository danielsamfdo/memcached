#ifndef MEMCACHED_CLONE_BRUTE_FORCE_STR_UTIL_H
#define MEMCACHED_CLONE_BRUTE_FORCE_STR_UTIL_H

using namespace std;

//Ref - http://stackoverflow.com/a/20446239/1026535
bool ends_with(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

#endif //MEMCACHED_CLONE_BRUTE_FORCE_STR_UTIL_H