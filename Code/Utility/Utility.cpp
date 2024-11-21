#include "../UtilityHeader/Utility.hpp"

int ft_strncmp(std::string _path, std::string _configPath) {
    int i = -1;
    for(i = 0; _path[i] && _configPath[i]; i++) {
        if (_path[i] != _configPath[i])
            break;
    }
    return i;
}

bool isDirectory(const std::string& path) {
	struct stat info;

	if (!stat(path.c_str(), &info)) {
		if (S_ISDIR(info.st_mode)) {
			return true;
		}
	}
	return false;
}

int checkPermissions(const std::string& path) {
	if (!access(path.c_str(), F_OK)) {
		if (!access(path.c_str(), R_OK)) return 0;
		else return 403;
	}
	return 404;
}

std::string normalizedPath(const std::string& _path, const std::string& _root) {
    std::string path, temp;

    for (size_t i = 0, j = 0; i < _path.size(); ++i) {
        if (i && (temp[j] == '/' && _path[i] == '/')) continue;
        temp += _path[i];
        j++;
    }

	return _root + temp;
}

std::string getGMTDate() {
	time_t now = time(0);
	struct tm *gmtm = gmtime(&now);
	char date_str[100];
	strftime(date_str, sizeof(date_str), "%a, %d %b %Y %H:%M:%S GMT", gmtm);
	return std::string(date_str);
}
