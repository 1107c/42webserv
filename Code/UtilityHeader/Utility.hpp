#ifndef UTILITY_HPP
#define UTILITY_HPP

# include <sstream>
# include <stddef.h>
# include <unistd.h>
# include <sys/stat.h>

template <typename T>
std::string ToString(const T& num) {
	std::ostringstream oss;
    oss << num;
    return oss.str();
}

bool isDirectory(const std::string& path);
int checkPermissions(const std::string& path);
std::string normalizedPath(const std::string& _path, const std::string& _root);
std::string getGMTDate();
int ft_strncmp(std::string _path, std::string _configPath);

#endif