#ifndef UTILITY_HPP
#define UTILITY_HPP

# include <sstream>
# include <iostream>
# include <stddef.h>
# include <unistd.h>
# include <vector>
# include <sys/stat.h>

template <typename T>
std::string ToString(const T& num) {
	std::ostringstream oss;
    oss << num;
    return oss.str();
}

bool isDirectory(const std::string& path);
int checkPermissions(const std::string& path);
std::string setNormalizedPath(const std::string& _path, const std::string& _root);
std::string getGMTDate();
int ft_strncmp(std::string _path, std::string _configPath);
void getArgv(std::vector<std::string>& argv, const std::string& str);
std::string createContentLength(const std::string& str);

#endif