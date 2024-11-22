#include "../UtilityHeader/Utility.hpp"

int ft_strncmp(std::string _path, std::string _configPath) {
    // int i = -1;
    // for(i = 0; _path[i] && _configPath[i]; i++) {
    //     if (_path[i] != _configPath[i])
    //         break;
    // }
	int i= 0;
	while (_path[i] && _configPath[i] && _configPath[i] == _path[i])
		++i;
	if (_path[i])
		return -1;
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

std::string setNormalizedPath(const std::string& _path, const std::string& _root) {
    std::string path, temp;

    for (size_t i = 0, j = 0; i < _path.size(); ++i) {
        if (i && (temp[j] == '/' && _path[i] == '/')) continue;
        temp += _path[i];
        j++;
    }

	path = _root + temp;
	if (isDirectory(path)) {
		
	}
	return path;
}

std::string getGMTDate() {
	time_t now = time(0);
	struct tm *gmtm = gmtime(&now);
	char date_str[100];
	strftime(date_str, sizeof(date_str), "%a, %d %b %Y %H:%M:%S GMT", gmtm);
	return std::string(date_str);
}

void getArgv(std::vector<std::string>& argv, const std::string& str) {
	size_t size = 0;
	size_t pos = str.find('=');
	while (pos != std::string::npos) {
		size++;
		pos = str.find('=', pos + 1);
	}
	std::string temp = str;
	for (size_t i = 0; i < size; ++i) {
		size_t start = temp.find('=');
		size_t endpos;
		endpos = temp.find('&', start);
		if (endpos != std::string::npos) {
			argv.push_back((temp.substr(start + 1, endpos - start - 1)));
			temp = temp.substr(endpos + 1);
		}
		else {
			argv.push_back(temp.substr(start + 1));
		}
	}
}

std::string createContentLength(const std::string& str) {
	size_t pos = str.find("\r\n\r\n");
	size_t size = str.substr(pos + 4).length();

	std::string result = "Content-Length: " + ToString(size) + "\r\n";
	return result;
}
