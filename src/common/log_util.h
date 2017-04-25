//Ref - http://stackoverflow.com/a/2235618/1026535
#ifndef MEMCACHED_CLONE_BRUTE_FORCE_LOG_UTIL_H
#define MEMCACHED_CLONE_BRUTE_FORCE_LOG_UTIL_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

class Logger {
private:
    ostringstream oss;
    string level;


public:
    template <typename T>
    Logger& operator<<(T a);

    Logger(string a_level) {
        level = a_level;
    }

    Logger& operator<<( ostream&(*f)(ostream&) )
    {
        if( f == static_cast<decltype(f)>(std::endl) )
        {
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);

            if(level == string("ERROR")) {
                cerr << put_time(&tm, "%d-%m-%Y %H-%M-%S") << " [" << level << "] - " << oss.str() << endl;
            }
            else {
                cout << put_time(&tm, "%d-%m-%Y %H-%M-%S") << " [" << level << "] - " << oss.str() << endl;
            }

            oss.str("");
        }
        return *this;
    }
};

template <typename T>
Logger& Logger::operator<<(T a) {
    oss << a;
    return *this;
}

Logger log_error("ERROR");
Logger log_info("INFO");

#endif //MEMCACHED_CLONE_BRUTE_FORCE_LOG_UTIL_H