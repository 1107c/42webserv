#include "../inc/ServerSocket.hpp"

// Methods를 비교하기 위한 Methods 리스트
// 모든 HTTP Method들의 집합
const std::string ServerSocket::methodList[] = {
	 "GET", "POST", "DELETE", "PUT", "PATCH", "HEAD", "OPTIONS", "TRACE", "CONNECT", ""
};

// void	ServerSocket::closeClient(size_t index, int fd, std::vector<bool>& response_needed) {
// 	close(_fds[index].fd);
// 	_fds.erase(_fds.begin() + index);
// 	response_needed.erase(response_needed.begin() + index);
// 	_cliMap.erase(fd);
// }

// void	ServerSocket::serverSockSet(const std::vector<Server>& ser) {
// 	std::map<u_int16_t, int> portCheck;
// 	size_t size = ser.size();
// 	for (size_t i = 0; i < size; ++i) {
// 		for (size_t j = 0; j < ser[i].getPortSize(); ++j) {
// 			if (!portCheck.empty()) {
// 				if (portCheck.find(ser[i].getPort(j)) != portCheck.end())
// 					continue ;
// 			}
// 			portCheck[ser[i].getPort(j)] = 1;
// 			int listenSock = socket(AF_INET, SOCK_STREAM, 0);
// 			if (listenSock == 0)
// 				throw std::runtime_error("Socket creation failed");
			
// 			int flags = fcntl(listenSock, F_GETFL, 0);
// 			fcntl(listenSock, F_SETFL, flags | O_NONBLOCK);

// 			int opt = 1;
// 			if (setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
// 				throw std::runtime_error("setsockopt failed");
			
// 			struct sockaddr_in address;
// 			std::string ip = ser[i].getHost();
// 			in_addr_t ipAddr = inet_addr(ip.c_str());
// 			if (ipAddr == INADDR_NONE)
// 				throw std::invalid_argument("Address Invalid");
// 			memset(&address, 0, sizeof(address));
// 			address.sin_family = AF_INET;
// 			address.sin_addr.s_addr = ipAddr;
// 			address.sin_port = htons(ser[i].getPort(j));

// 			if (bind(listenSock, (struct sockaddr *)&address, sizeof(address)) < 0)
// 				throw std::runtime_error("Bind Failed");
			
// 			if (listen(listenSock, SOMAXCONN) < 0)
// 				throw std::runtime_error("Listen Failed");
			
// 			struct pollfd listen_pollfd;
// 			listen_pollfd.fd = listenSock;
// 			listen_pollfd.events = POLLIN;
// 			_fds.push_back(listen_pollfd);
// 		}
// 	}
// }

// void	ServerSocket::connectClient(std::vector<Server>& ser) {
// 	std::vector<bool> response_needed(_fds.size(), false);
// 	size_t size = _fds.size();
// 	while (true) {
// 		int ret = poll(&_fds[0], _fds.size(), 0);
// 		if (ret < 0)
// 			throw std::runtime_error("poll Failed");
// 		for (size_t i = 0; i < size; ++i) {
// 			if (_fds[i].revents & POLLIN) {
// 				struct sockaddr_in client_addr;
// 				socklen_t client_len = sizeof(client_addr);
// 				int client_sock = accept(_fds[i].fd, (struct sockaddr*)&client_addr, &client_len);
// 				if (client_sock < 0) {
// 					std::cerr << "Aceept failed" << std::endl;
// 					continue ;
// 				}
// 				int client_flags = fcntl(client_sock, F_GETFL, 0);
// 				fcntl(client_sock, F_SETFL, client_flags | O_NONBLOCK);

// 				struct pollfd client_pollfd;
// 				client_pollfd.fd = client_sock;
// 				client_pollfd.events = POLLIN;
// 				_fds.push_back(client_pollfd);
// 				response_needed.push_back(false);
// 			}
// 		}
// 		clientRequest(response_needed, ser, size);
// 	}
// }

// 위까지는 이전 사용했던 poll 이기 때문에 주석 처리함 epoll을 이용할 것 이기 때문에 필요없음
// 마지막 clientRequest에 server가 모두 담겨있는 구조체 ser와 server의 size를 줬음.
// 서버의 size를 준 이유는 _fds vector에 서버 및 클라이언트 모두 담아서 사용중, size만큼은 server size < client가 됨
void	ServerSocket::clientRequest(std::vector<bool>& response_needed, std::vector<Server>& ser, const size_t& size) {
	for (size_t i = size; i < _fds.size();) {
		Client cli;
		int fd = _fds[i].fd;
		if (_fds[i].revents & (POLLERR | POLLHUP)) {
                std::cout << ((_fds[i].revents & POLLERR) ? "Error" : "Connection close") << std::endl;
				closeClient(i, fd, response_needed);
				continue;
        }

		try {
			if (_fds[i].revents & POLLIN) {
				std::vector<char> totalData = readClientData(_fds[i].fd, ser, cli);
				if (totalData.empty()) {
					closeClient(i, fd, response_needed);
					continue;
				}
				response_needed[i] = true;
				_fds[i].events |= POLLOUT;
				cli.setMethod(determineHttpMethod(totalData));						// 해당 client의 request를 모두 읽었을 시 해당 client class의 _methods값 설정.(요청이 GET POST DELETE인지 구분)
				cli.setData(totalData);												// 해당 client의 request를 해당 client class의 data에 입력.
				_cliMap[fd] = cli;
			}

			if (_fds[i].revents & POLLOUT && response_needed[i]) {
				handleHttpMethod(fd, ser);									// client의 fd 및 Server정보를 보냄. 이후 메서드에서 GET POST DELETE로 분기됨.
				response_needed[i] = false;
				_fds[i].events &= ~POLLOUT;
			}
			++i;
		} catch (const std::exception& e) {
			std::cerr << "Error client " << fd << ": " << e.what() << std::endl;
			closeClient(i, fd, response_needed);
			continue;
		}
    }
}

// poll 사용할 당시 모든 데이터를 한번에 읽은 후 데이터를 반환했음 EageTrigger처럼 한번의 요청에 모든 데이터를 읽음
// 이후 우리가 사용할 것은 epoll LT 예상 코드는 넘어가고 check하는 부분 부터 확인
std::vector<char> ServerSocket::readClientData(int fd, std::vector<Server>& ser, Client& cli) {
    std::vector<char> buffer(8192);
    std::vector<char> totalData;
    size_t totalRead = 0;

    while (true) {
        int valread = recv(fd, &buffer[0], buffer.size(), 0);
        if (valread > 0) {
            totalData.insert(totalData.end(), buffer.begin(), buffer.begin() + valread);
            totalRead += valread;
        } else {
            if (valread < 0 && (errno != EAGAIN && errno != EWOULDBLOCK)) {
				return std::vector<char>();
			}
            break;
        }
        if (cli.getSize() == __SIZE_MAX__) {
            configureCheck(totalData, ser, cli);
        } else {
            if (totalRead > cli.getSize())
                throw std::runtime_error("maxBodySize exceeded. This exception will trigger an error page.");
        }
    }
    return totalData;
}

// request를 data로 담아뒀던걸 stringstream으로 변경 후 한줄씩 검사 진행
bool	ServerSocket::configureCheck(std::vector<char> data, std::vector<Server>& ser, Client& cli) {
	std::string dataStr(data.begin(), data.end());
	std::istringstream iss(dataStr);
	std::string line;

	if (!std::getline(iss, line) || !checkRequestLine(line, cli))	// 최초 [Methods    Location     HTTP/version] 구문 검사
		throw std::runtime_error("400 or getline Error");
	bool hostFound = false;
	while (std::getline(iss, line)) {
		if (line.substr(0, 6) == "Host: ") {	// 서버에 연결될 ip 찾기 즉 Host 찾는구문
			hostFound = true;
			if (!checkHostHeader(line.substr(6), ser, cli)) {	// "Host: " 을 제외하고 ip 및 port만 인자로 줌
				throw std::runtime_error("host 400 Error exception");
			}
			break ;
		}
	}
	if (!hostFound)
		throw std::runtime_error("host missing");
	return true;
}

// HTTP 요청 라인 검사
bool	ServerSocket::checkRequestLine(const std::string& line, Client& cli) {
	std::istringstream iss(line);
	std::string method, uri, httpVersion;
	iss >> method >> uri >> httpVersion;
	bool methodCheck = false;

	for (size_t i = 0; i < 9; ++i) {			// 현재 요청의 Method를 최상단 정의했던 Methods 리스트들과 비교 존재한다면 이상없는 요청라인
		if (method == methodList[i]) {
			methodCheck = true;
			break ;
		}
	}
	if (!methodCheck)							// Methods 리스트에 없는 Method는 잘못된 Method false return
		return false;
	if (uri.empty() || uri[0] != '/')			// Location 부분이 비어있다거나 첫번째가 /로 시작하지 않으면 false return
		return false;
	if (httpVersion != "HTTP/1.0" && httpVersion != "HTTP/1.1")		// version은 0.9를 제외하고 1.0 및 1.1만 관리 직접비교했음. 0.9를 제외했던 이유 생각이 안남 이유가 있었던것으로 판단..
		return false;
	cli.setVersion(httpVersion);
	cli.setOriginUri(uri);
	return true;
}


bool	ServerSocket::checkHostHeader(const std::string& hostLine, const std::vector<Server>& ser, Client& cli) {
	std::string::size_type pos = hostLine.find(':');			// 포트가 지정되었는지 확인
	std::string serverName = (pos != std::string::npos) ? hostLine.substr(0, pos) : hostLine;
	long portNum = 0;

	// port가 있다면 port 양식에 맞게 작성되었는지, 되었다면 숫자로 변경
	if (pos != std::string::npos) {
		std::string portStr = hostLine.substr(pos + 1);
		char* endptr;
		portNum = std::strtol(portStr.c_str(), &endptr, 10);
		if ((*endptr != '\r' && *endptr != '\n' && *endptr != ';' && *endptr != '\0') || portNum <= 0 || portNum > 65535) {
			return false;
		}
	} else return false;

	// Server_Block에 해당하는 ServerName과 Port 찾기 중복된게 있다면 NGINX는 위에것을 사용하는것으로 알고있음
	// 먼저 발견되는 Server_block을 사용할것임
	for (size_t i = 0; i < ser.size(); ++i) {
		for (size_t j = 0; j < ser[i].getNameSize(); ++j) {
			if (serverName == ser[i].getName(j)) {
				for (size_t k = 0; k < ser[i].getPortSize(); ++k) {
					if (portNum == ser[i].getPort(k)) {
						cli.setSize(ser[i].getSize());				// 찾았다면 기본정보 입력
						cli.setIndex(i);
						cli.setPort(portNum);
						return true;
					}
				}
			}
		}
	}
	return false;
}

// Methods가 어떤것이 왔는지 확인하는 부분 subject에 따라 GET POST DELETE만 이용할 것임 
// 허용되지 않은 메소드라면 404가 아닌 405 오류로 기억 코드에 GET POST DELETE 셋 모두 아니고 methodlist에 존재하지 않는것이라면 405? 404? 의논 필요
std::string ServerSocket::determineHttpMethod(const std::vector<char>& totalData) {
    std::string dataStr(totalData.begin(), totalData.end());
    for (size_t i = 0; i < 9; ++i) {
        size_t pos = dataStr.find(methodList[i]);
        if (pos != std::string::npos) {
            if (methodList[i] != "GET" && methodList[i] != "POST" && methodList[i] != "DELETE")
                throw std::runtime_error("404 error. This exception will trigger an error page");
            return methodList[i];
        }
    }
    throw std::runtime_error("Unknown HTTP method");
}

// Location을 실제 경로로 해석 하고 GET POST DELETE에 해당하는 Method에 따라 보내주기
void ServerSocket::handleHttpMethod(int fd, std::vector<Server>& ser) {
	uriMapping(fd, ser);
    if (_cliMap[fd].getMethod() == "GET") {
        handleGetRequest(fd, ser);
    } else if (_cliMap[fd].getMethod() == "POST") {
        // handlePostRequest(cli, data, ser);
    } else if (_cliMap[fd].getMethod() == "DELETE") {
        // handleDeleteRequest(cli, data, ser);
    }
}

// GET 요청일 경우
// Directory와 File 처리기를 따로 해야할지 한번에 할수 있을지 잘 모르겠음, 논의 후 맞는 방향으로 재구성 필요
void ServerSocket::handleGetRequest(int fd, std::vector<Server>& ser) {
	std::string response;
	int			result = isDirectory(_cliMap[fd].getMappingUri());	// Mapping한 Location Directory인지 file인지 판단
	if (result == 1) {
		response = directoryHandler(ser, fd);	// Directory 처리기로 이동
	} else if (result == -1) {
		response = errorResponse("404", ser, fd); // 찾을 수 없는 경로라면 404
	}
	// else
	// 	fileHandler(response);
	int	valsend = send(fd, response.c_str(), response.length(), 0);
	if (valsend > 0) {
		std::cout << "Response sent" << std::endl;
	};
}

// Response를 줄때의 시간 입력을 위해 만듬
std::string ServerSocket::getGMTDate() {
	time_t now = time(0);
	struct tm *gmtm = gmtime(&now);
	char date_str[100];
	strftime(date_str, sizeof(date_str), "%a, %d %b %Y %H:%M:%S GMT", gmtm);
	return std::string(date_str);
}

// Directory인지 아닌지 판별하는 곳
// /보다 .이 뒤에 있으면 무조건 파일이니 비교 하지않고 바로 파일 반환
// ex) webserv/other/test.txt          << .이 / 뒤에 존재 파일로 인식
int	ServerSocket::isDirectory(const std::string& uri) {
	size_t dotPos = uri.find_last_of('.');
	size_t slashPos = uri.find_last_of('/');
	if (dotPos != std::string::npos && dotPos > slashPos) {
		return 0;
	}
	struct stat info;
	if (stat(uri.c_str(), &info) != 0) {
		return -1;
	} else if (info.st_mode & S_IFDIR) {
		return 1;
	} else {
		return 0;
	}
	return 0;
}

// 폴더일때의 처리
// 폴더 파일을 따로 나눠야하는지 의논 필요 이부분은 아직 확인할 필요 없을듯함
std::string	ServerSocket::directoryHandler(std::vector<Server>& ser, int fd) {
	struct stat			info;
	size_t				index = _cliMap[fd].getIndex();
	Location			loca = ser[index].getLocation(_cliMap[fd].getOriginUri());
	std::string			path = loca.getRoot() + "/";
	std::ostringstream	oss;
	std::string			tmp = _cliMap[fd].getData();
	std::string			response;

	for (size_t i = 0; i < loca.getIndexSize(); ++i) {
		std::string temp = path + loca.getIndex(i);
		if (stat(temp.c_str(), &info) == 0) {
			path = temp;
			long size = info.st_size;
			oss << size;
		}
	}
	if (path == loca.getRoot() + "/") {
		return errorResponse("404", ser, fd);
	}
	response = "HTTP/1.1 200 OK\r\nDate: ";
	response += getGMTDate() + "\r\n";

	if (path.find("htm") != std::string::npos) {
		response += "Content-Type: text/html\r\nContent-Length: ";
		response += oss.str() + "\r\nConnection: ";
	}
	response += connectionSelect(fd);
	response += getContent(path);
	return response;
}

// void	ServerSocket::fileHandler(std::string& response) {

// }

// 현재는 404만 있음 이 부분도 에러 어떻게 처리할지 의논
std::string	ServerSocket::errorResponse(const std::string& number, std::vector<Server>& ser, int fd) {
	struct stat info;
	std::string response;
	std::ostringstream oss;
	std::string tmp = _cliMap[fd].getData();
	
	if (number == "404") {
		std::string path = ser[_cliMap[fd].getIndex()].getError("404", 0);
		response = "HTTP/1.1 404 Not Found\r\nDate: ";
		response += getGMTDate() + "\r\nContent-Type: text/html\r\nContent-Length: ";
		if (stat(path.c_str(), &info) == 0) {
			long size = info.st_size;
			oss << size;
		}
		response += oss.str() + "\r\nConnection: ";
		response += connectionSelect(fd);
		response += getContent(path);
	}
	return response;
}

// Connection 헤더가 keep-alive인지 close인지 확인 후 반환
std::string ServerSocket::connectionSelect(int fd) {
	std::string tmp = _cliMap[fd].getData();
	std::string response;
	size_t pos = tmp.find("Connection: ");
	if (pos != std::string::npos) {
		size_t start = pos + 12;
		size_t end = tmp.find("\r\n", start);
		response = tmp.substr(start, end - start) + "\r\n\r\n";
	} else {
		if (_cliMap[fd].getVersion() == "HTTP/1.1")
			response += "keep-alive\r\n";
		else if (_cliMap[fd].getVersion() == "HTTP/1.0")
			response += "close\r\n\r\n";
	}
	return response;
}

// 파일 내용을 제외한 부분은 밖에서 처리 후 파일 내용만 이쪽에서 return 하는것
// 반복되는 내용이라 method 관리했음
std::string ServerSocket::getContent(const std::string& path) {
	std::ifstream	file(path.c_str());
	std::string		line;
	std::string		result;
	while (getline(file, line)) {
		result += line + "\n";
	}
	file.close();
	return result;
}

// Location에 해당하는 실제 root 맵핑해주기 ex) Location / { root /webserv/html } /webserv/html < 구하는 곳
void	ServerSocket::uriMapping(int fd, std::vector<Server>& ser) {
	std::string	uri = _cliMap[fd].getOriginUri();
	std::string	mapUri;
	int			size = std::count(uri.begin(), uri.end(), '/');
	int			index = _cliMap[fd].getIndex();

	for (int i = 0; i < size; ++i) {
		std::string	temp;
		size_t		posTemp;
		size_t		pos;
		if (i == 0)
			temp = uri;
		else {
			posTemp = uri.find_last_of('/');
			pos = uri.find_last_of('/', posTemp - i + 1);
			temp = uri.substr(0, pos);
		}
		Location loca = ser[index].getLocation(temp);
		if (!loca.getPath().empty()) {
			if (i == 0) {
				mapUri = loca.getRoot();
			}
			else {
				mapUri = loca.getRoot();
				mapUri += uri.substr(pos);
			}
			_cliMap[fd].setMappingUri(mapUri);
			return ;
		}
	}
	mapUri = ser[index].getLocation("/").getRoot() + uri;
	_cliMap[fd].setMappingUri(mapUri);
}