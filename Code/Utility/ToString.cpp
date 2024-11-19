#include "../UtilityHeader/ToString.hpp"

std::string ToString(int num) {
    std::ostringstream oss;
    oss << num;
    std::string st = oss.str();
    return st;
}

std::string ToString(long num) {
    std::ostringstream oss;
    oss << num;
    std::string st = oss.str();
    return st;
}

std::string ToString(unsigned long long num) {
    std::ostringstream oss;
    oss << num;
    std::string st = oss.str();
    return st;
}

std::string ToString(unsigned int num) {
    std::ostringstream oss;
    oss << num;
    std::string st = oss.str();
    return st;
}

std::string ToString(size_t num) {
    std::ostringstream oss;
    oss << num;
    std::string st = oss.str();
    return st;
}

int ft_strncmp(std::string _path, std::string _configPath) {
    int i = -1;
    for(i = 0; _path[i] && _configPath[i]; i++) {
        if (_path[i] != _configPath[i])
            break;
    }
    return i;
}
