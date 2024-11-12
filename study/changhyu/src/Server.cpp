#include "../inc/Server.hpp"
#include "../inc/Location.hpp"

//Nginx 기준 maxBodySize가 1MB였던것을 확인해서 설정 하지 않은 기본 값을 1MB로 설정
Server::Server()
	: _serverName(), _port(), _host(""), _maxBodySize(1024 * 1024), _root(""), 
	_methods(), _autoidx(false), _index(), _errorPage(), _locationMap() {}

Server::~Server(){}

Server::Server(const Server& rhs)
	: _serverName(rhs._serverName), _port(rhs._port), _host(rhs._host), _maxBodySize(rhs._maxBodySize), _root(rhs._root), 
	_methods(rhs._methods), _autoidx(rhs._autoidx), _index(rhs._index), _errorPage(rhs._errorPage) {
		std::map<std::string, Location>::const_iterator it;
		for (it = rhs._locationMap.begin(); it != rhs._locationMap.end(); ++it)
			_locationMap[it->first] = it->second;
	}

Server&	Server::operator=(const Server& rhs) {
	if (this != &rhs) {
		_serverName = rhs._serverName;
		_port = rhs._port;
		_host = rhs._host;
		_maxBodySize = rhs._maxBodySize;
		_root = rhs._root;
		_methods = rhs._methods;
		_autoidx = rhs._autoidx;
		_index = rhs._index;
		_errorPage = rhs._errorPage;
        _locationMap.clear();
		std::map<std::string, Location>::const_iterator rhs_it;
        for (rhs_it = rhs._locationMap.begin(); rhs_it != rhs._locationMap.end(); ++rhs_it) {
            _locationMap[rhs_it->first] = rhs_it->second;
        }
	}
	return *this;
}

// Host의 IP를 옥텟 당 검사한 로직 0~255.0~255.0~255.0~255 부합하는지
bool isValidOctet(const char* str, char** endptr) {
	errno = 0;
	long value = std::strtol(str, endptr, 10);
	return errno != ERANGE && value >= 0 && value <= 255;
}

// 세미콜론 이후 값이 있다면 공백인지 다른값인지 확인, 공백이라면 true 다른 값이 나온다면 false
// 세미콜론이 없다면 true
bool isValidStrMini(const std::string& str) {
	size_t	pos = str.find(";");
	if (pos != std::string::npos && str[pos + 1] != '\0') {
		while (str[++pos]) {
			if (!std::isspace(str[pos]))
				return false;
		}
	}
	return true;
}

// 헤더 이후의 값들의 개수를 셈, 예를 들어 index, serverName같이 다수의 값이 올수 있는것들의 크기 지정
bool isValidStr(const std::string& str) {
	bool	flags = false;
	int		cnt = 0;
	if (!isValidStrMini(str))
		return false;
	size_t	pos = str.find(";");
	if (pos == std::string::npos) {
		pos = str.length();
	}
	for (std::string::const_iterator it = str.begin(); it != str.begin() + pos; ++it) {
		if (flags == false) {
			if (!std::isspace(*it)) {
				flags = true;
				cnt++;
			}
			continue ;
		}
		if (flags && std::isspace(*it))
			flags = false;
	}
	if (cnt != 1)
		return false;
	return true;
}

// ServerName 헤더 부분의 값 입력하는 부분
//앞에 공백 건너뛰기 후 다음 구분자인 공백 이나 세미콜론을 만나기전 까지 복사 후 완료되면 _serverName 벡터에 추가
// ServerName은 다수의 값이 올수 있기 때문
void	Server::setName(const std::string& str) {
	std::string	temp;
	size_t		i = 0;
	if (!isValidStrMini(str))
		return ; // << throw expected
	while (i < str.length()) {
		while (i < str.length() && std::isspace(str[i]))
			i++;
		while (i < str.length() && !std::isspace(str[i]) && str[i] != ';')
			temp += str[i++];
		if (!temp.empty()) {
			_serverName.push_back(temp);
			temp.clear();
		}
		if (str[i++] == ';')
			break ;
	}
	if (_serverName.empty())
		return ;// < throw expected
}

// Port 값을 등록. 위와 같이 공백 건너뛰기, 유효한 포트값인지 확인 후 등록
void	Server::setPort(const std::string& str) {
	std::string temp;
	size_t		i = 0;
	long		check;

	while (i < str.length() && std::isspace(str[i]))
		i++;
	while (i < str.length() && !std::isspace(str[i]) && str[i] != ';')
		temp += str[i++];
	if (!temp.empty()) {
		char* endptr;
		errno = 0;
		check = std::strtol(temp.c_str(), &endptr, 10);
		if (endptr == temp.c_str() || errno == ERANGE || (check < 0 || check > 65535))
			return ; // throw expected
	}
	while (i < str.length() && std::isspace(str[i]))
		i++;
	if (i < str.length()) {
		if (str[i] == ';') {
			i++;
			while (i < str.length() && std::isspace(str[i]))
				i++;
			if (i < str.length())
				return ; // throw expected
		} else {
			return ; // throw expected
		}
	}
	_port.push_back(static_cast<u_int16_t>(check));
}

// Host 값을 등록. 위와 같이 공백 건너뛰기 후 유효값인지 확인 후 등록
void	Server::setHost(const std::string& str) {
	std::string temp;
	size_t		i = 0;
	while (i < str.length() && std::isspace(str[i]))
		i++;
	while (i < str.length() && !std::isspace(str[i]) && str[i] != ';')
		temp += str[i++];
	while (i < str.length() && std::isspace(str[i]))
		i++;
	if (i < str.length()) {
		if (str[i] == ';') {
			i++;
			while (i < str.length() && std::isspace(str[i]))
				i++;
			if (i < str.length()) 
				return ; // throw expected
		} else
			return ; // throw expected
	}
	if (!temp.empty()) {
		char* endptr;
		const char* current = temp.c_str();
		for (int j = 0; j < 4; ++j) {
			if (!isValidOctet(current, &endptr))
				return ; // throw expected
			if (j < 3) {
				if (*endptr != '.')
					return ; // throw expected
				current = endptr + 1;
			} else {
				while (*endptr != '\0') {
					if (!std::isspace(*endptr))
						return ;// throw expected
					endptr++;
				}
			}
		}
		_host = temp;
	} else
		return ; // throw expected
}

// MaxBodySize를 등록. 각 크기마다 해당하는 바이트 반환
void	Server::setSize(const std::string& str) {
	char*	endptr;
	long	temp = std::strtol(str.c_str(), &endptr, 10);

	if (temp > 0 && *endptr != '\0') {
		std::string unit;
		unit.assign(endptr);
		for (std::string::iterator it = unit.begin(); it != unit.end();) {
			if (std::isspace(*it) || *it == ';')
				it = unit.erase(it);
			else
				it++;
		}
		if (unit == "M" || unit == "MB") {
		temp *= 1024 * 1024;
		}else if (unit == "K" || unit == "KB") {
			temp *= 1024;
		}else if (unit == "G" || unit == "GB") {
			temp *= 1024 * 1024 * 1024;
		} else
			return ; // throw expected
		_maxBodySize = temp;
	}
}

// Root 등록. 공백건너뛰기
void	Server::setRoot(const std::string& str) {
	if (!isValidStr(str))
		return ; // throw expected
	std::string temp;
	size_t		i = 0;
	while (i < str.length() && std::isspace(str[i]))
		i++;
	while (i < str.length() && !std::isspace(str[i]) && str[i] != ';')
		temp += str[i++];
	_root = temp;
}

// Errorpage 헤더 해석해서 등록.
// Errorpage를 어떻게 해야할지, 모두 각자 관리해야할지 정하지 못하여서 미완성이라고 보면 됨
// Errorpage 처리 방법 의논 필요
void	Server::setError(const std::string& str) {
	std::string	key;
	std::string	value;
	size_t		i = 0;
	if (!isValidStrMini(str))
		return ; // << throw expected
	while (i < str.length() && std::isspace(str[i]))
		i++;
	while (i < str.length() && !std::isspace(str[i]) && str[i] != ';')
		key += str[i++];
	if (key.empty() || str[i] == ';')
		return ; // < throw expected
	while (i < str.length()) {
		while (i < str.length() && std::isspace(str[i]))
			i++;
		while (i < str.length() && !std::isspace(str[i]) && str[i] != ';')
			value += str[i++];
		if (!value.empty()) {
			_errorPage[key].push_back(value);
			value.clear();
		}
		if (str[i++] == ';')
			break ;
	}
}

// Index 등록. 다수의 값이 올수 있는 헤더이니 vector로 관리함
void	Server::setIndex(const std::string& str) {
	std::string	temp;
	size_t		i = 0;
	if (!isValidStrMini(str))
		return ; // << throw expected
	while (i < str.length()) {
		while (i < str.length() && std::isspace(str[i]))
			i++;
		while (i < str.length() && !std::isspace(str[i]) && str[i] != ';')
			temp += str[i++];
		if (!temp.empty()) {
			_index.push_back(temp);
			temp.clear();
		}
		if (str[i++] == ';')
			break ;
	}
	if (_index.empty())
		return ;// < throw expected
}

// SetAutoindex 등록. 공백 건너뛰기
void	Server::setAutoidx(const std::string& str) {
	std::string temp;
	size_t		i = 0;
	if (!isValidStr(str))
		return ; // throw expected
	while (i < str.length() && std::isspace(str[i]))
			i++;
	while (i < str.length() && !std::isspace(str[i]) && str[i] != ';')
			temp += str[i++];
	if (temp == "on")
		_autoidx = true;
	else
		_autoidx = false;
}

// 허용가능한 메소드 등록. 다수의 값이 올수 있으니 벡터로
void	Server::setMethods(const std::string& str) {
	std::string	temp;
	size_t		i = 0;
	if (!isValidStrMini(str))
		return ; // << throw expected
	while (i < str.length()) {
		while (i < str.length() && std::isspace(str[i]))
			i++;
		while (i < str.length() && !std::isspace(str[i]) && str[i] != ';')
			temp += str[i++];
		if (!temp.empty()) {
			_methods.push_back(temp);
			temp.clear();
		}
		if (str[i++] == ';')
			break ;
	}
	if (_methods.empty())
		return ;// < throw expected
}

// 해당 Server_block에 존재하는 Location들을 자식 클래스로 만들어서 Map에 등록중 Map을 굳이 사용해야하나 ? 라는 의문 의논 필요
void	Server::setLocation(const std::string& str, const Location& loca) {
	_locationMap[str] = loca;
};

// 해당 인덱스에 맞는 ServerName 반환 
// serverName localhost 127.0.0.1 이라면 0이 localhost 1이 127.0.0.1
std::string	Server::getName(size_t i) const {
	if (_serverName.empty())
		return "server_name empty.";
	return _serverName[i];
}

// 위와 같이 Port반환
u_int16_t	Server::getPort(size_t i) const {
	if (_port.empty())
		return 0;
	return _port[i];
}

// Host 반환
std::string	Server::getHost() const {
	if (_host.empty())
		return "host empty.";
	return _host;
};

// maxBodySize반환
long		Server::getSize() const {
	return _maxBodySize;
};

// root 반환
std::string	Server::getRoot() const {
	if (_root.empty())
		return "root empty.";
	return _root;
};

// 해당 인덱스의 Methods 반환
std::string	Server::getMethods(size_t i) const {
	if (_methods.empty())
		return "methods empty.";
	return _methods[i];
};

// 해당 페이지 AutoIdx 반환
bool		Server::getAutoidx() const {
	return _autoidx;
};

// 해당 인덱스의 Index 반환
// index index.html index.htm home.html 0이라면 index.html 반환
std::string	Server::getIndex(size_t i) const {
	if (_index.empty())
		return "Error: index empty.";
	return _index[i];
};

// 이전 말했듯 ErrorPage를 모두 개별적으로 관리한다면 거기에 알맞는 ErrorPage 반환하는 부분
std::string	Server::getError(const std::string& rhs, size_t i) {
	if (_errorPage.empty())
		return "ErrorPage empty.";
	if (_errorPage.find(rhs) == _errorPage.end())
		return "Not found key.";
	std::vector<std::string> temp = _errorPage[rhs];
	return	temp[i];
};

// Location 이름에 맞는 Location 반환
Location Server::getLocation(const std::string& rhs) {
	if (_locationMap.empty())
		throw std::invalid_argument("LocationMap empty.");
	if (_locationMap.find(rhs) == _locationMap.end())
		return Location();
	Location temp = _locationMap[rhs];
	return temp;
}

// 다른 파일에서 private인 port에 접근하기 위해 port의 전체사이즈를 구함
size_t	Server::getPortSize() const {
	return _port.size();
}

// 다른 파일에서 private인 name에 접근하기 위해 name의 전체사이즈를 구함
size_t	Server::getNameSize() const {
	return _serverName.size();
}

// 다른 파일에서 private인 index에 접근하기 위해 index의 전체사이즈를 구함
size_t	Server::getIndexSize() const {
	return _index.size();
}

// 다른 파일에서 private인 methods에 접근하기 위해 methods의 전체사이즈를 구함
size_t Server::getMethodsSize() const {
	return _methods.size();
}

// 해당 하는 이름의 Location 존재하는지 확인
bool	Server::findLocation(std::string key) {
	std::map<std::string, Location>::iterator it = _locationMap.find(key);
	if (it != _locationMap.end()) return true;
	return false;
}