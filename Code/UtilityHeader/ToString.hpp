#ifndef TOSTRING_HPP
#define TOSTRING_HPP

# include <sstream>
# include <stddef.h>
# include <unistd.h>

std::string ToString(int num);
std::string ToString(long num);
std::string ToString(unsigned long long num);
std::string ToString(unsigned int num);
std::string ToString(size_t num);

#endif