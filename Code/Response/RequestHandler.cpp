#include "../Socket/Epoll.hpp"
#include "../UtilityHeader/Utility.hpp"

std::string Response::errorHandler(int error) {
	std::string path = getErrorPath(error);
	std::string header = getErrorHeader(error);
	std::ifstream file(path.c_str());
	if (!file.is_open()) {
        header = getErrorHeader(500);
        path = getErrorPath(500);
    }
	std::string html;
	std::string line;
	while (std::getline(file, line)) {
		html += line + "\n";
	}
	std::ostringstream ss;
	ss << html.size();

    std::cout << path << "\n" << header << std::endl;
	std::string response = header + "\r\n";
	response += "Content-Type: text/html; charset=UTF-8\r\n";
	response += "Content-Length: " + ss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Date: " + getGMTDate() + "\r\n\r\n";
	response += html;

	return response;
}

std::string Response::autoIndexFile(const Request& request) {
    size_t pos = request.getMappingUrl().rfind("/");
    std::string filename = request.getMappingUrl().substr(pos + 1);
    std::ifstream file(request.getMappingUrl().c_str(), std::ios::binary);
    if (!file.is_open()) return errorHandler(500);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0);
    std::string response;

    response += "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\n";
    response += "Content-Disposition: attachment; filename=\"" + filename + "\"\r\n";
    response += "Content-Length: " + ToString(size) + "\r\n";
    response += "Connection: keep-alive\r\n";
    response += getGMTDate() + "\r\n\r\n";

    response.reserve(response.size() + size);

    // 버퍼에 파일 내용 읽기
	const size_t buffer_size = 8192;
    std::vector<char> buffer(buffer_size);
    size_t bytes_read = 0;
	size_t remaining = size;
	while (remaining) {
		remaining -= bytes_read;
		size_t read_size = std::min(remaining, buffer_size);
		file.read(&buffer[0], read_size);
		bytes_read = file.gcount();
		// 이미지 데이터 추가
		response.append(&buffer[0], bytes_read);
	}

    return response;
}

std::string Response::autoIndexHandler(const Request& request) {
    std::string mapPath = request.getMappingUrl();
    DIR* dir = NULL;
    if (isDirectory(mapPath)) {
        dir = opendir(mapPath.c_str());
        if (!dir) {
            return errorHandler(500);
        }
    } else if (!isDirectory(mapPath) && request.getLocation().getAutoindex()) return autoIndexFile(request);
    std::string result;
    std::string path = request.getPath();
    struct dirent* entry;
    while ((entry = readdir(dir))) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue ;

        std::string filename;
        if (path[path.size() - 1] != '/') {
            filename += '/';
        }
        filename += entry->d_name;
        std::string _path = path + filename;
        std::string _mapPath = mapPath + filename;

        filename = entry->d_name;
        struct stat file_info;
        if (!stat(_mapPath.c_str(), &file_info)) {
            char time_str[100];
            struct tm* tm_info = localtime(&file_info.st_mtime);
            strftime(time_str, sizeof(time_str), "%Y년 %m월 %d일 %H시 %M분 %S초", tm_info);
            result += "<a href=\"" + _path;
            if (isDirectory(_path))
                result += "/";
            result += "\">" + filename + "</a> " + time_str + "\n";
        } else {
            return errorHandler(500);
        }
    }
    closedir(dir);
    return result;
}
std::string Response::redirectHandler(const std::string &mapPath, const std::string &code)
{
    std::string ss = code + " Moved Permanently";
    std:: string response = "HTTP/1.1 " + code + " Moved Permanently\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: " + ToString(ss.size()) + "\r\n";
    response += "Location: " + mapPath + "\r\n";
    response += "Connection: keep-alive\r\n";
	response += "Date: " + getGMTDate() + "\r\n\r\n";
    response += ss;
    return response;
}


std::string Response::textHandler(const Request& request, const std::string& accept) {
	std::string mapPath = request.getMappingUrl();
    std::string html;

	if (isDirectory(mapPath)) {
        html = autoIndexHandler(request);
    } else if (request.getLocation().getAutoindex()) {
        return autoIndexHandler(request);
    } else {
        std::ifstream file(request.getMappingUrl().c_str());
        if (!file.is_open()) return errorHandler(500);
            std::string line;
        while (std::getline(file, line)) {
            html += line + "\n";
        }
    }
    
    std::ostringstream ss;
    ss << html.size();
    
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: " + accept + "; charset=UTF-8\r\n";
    response += "Content-Length: " + ss.str() + "\r\n";
	response += "Date: " + getGMTDate() + "\r\n\r\n";
    response += html;

    return response;
}

std::string Response::imageHandler(const Request& request, const std::string& accept) {
	std::ifstream file(request.getMappingUrl().c_str(), std::ios::binary);
    if (!file.is_open()) return errorHandler(500);

    // 파일 크기 확인
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0);

	std::ostringstream oss;
    oss << size;

    // HTTP 응답 생성
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: " + accept + "\r\n";
    response += "Content-Length: " + oss.str() + "\r\n";
	response += "Date: " + getGMTDate() + "\r\n\r\n";

	response.reserve(response.size() + size);

    // 버퍼에 파일 내용 읽기
	const size_t buffer_size = 8192;
    std::vector<char> buffer(buffer_size);
    size_t bytes_read = 0;
	size_t remaining = size;
	while (remaining) {
		remaining -= bytes_read;
		size_t read_size = std::min(remaining, buffer_size);
		file.read(&buffer[0], read_size);
		bytes_read = file.gcount();
		// 이미지 데이터 추가
		response.append(&buffer[0], bytes_read);
	}

    return response;
}

std::string Response::postHandler(Request& request) {

    const std::string& body = request.getBody();
    std::string boundary;
    
    size_t pos = 0;
    std::string filename;
    std::string contentType;
    std::string fileData;

    pos = body.find("\r\n", pos);
    if (pos == std::string::npos)
	{		
    	std::cout<< "@@@@@@2"<<std::endl;
		return errorHandler(500);
	}
	boundary = body.substr(0, pos - 2);
	std::cout <<boundary <<std::endl;
    pos = pos + 2;  
    	std::cout<< "@@@@@@3"<<std::endl;
    while (pos < body.length()) {
        size_t lineEnd = body.find("\r\n", pos);
        if (lineEnd == std::string::npos) break; //캐리지 못찾을시
        std::string line = body.substr(pos, lineEnd - pos); // 캐리지제외라인
        
        if (line.find("Content-Disposition:") != std::string::npos) {
            size_t fnPos = line.find("filename=\"");
            if (fnPos != std::string::npos) {
                fnPos += 10; 
                size_t fnEnd = line.find("\"", fnPos);
                if (fnEnd != std::string::npos) {
                    filename = line.substr(fnPos, fnEnd - fnPos);
                }
            }
        }
        else if (line.find("Content-Type:") != std::string::npos) {
            contentType = line.substr(line.find(":") + 2);
        }
        else if (line.empty()) //r/n 하나 이후 실제이진파일 시작
		{
            pos = lineEnd + 2;  // 빈줄 \r\n 건너뛰기
            // 다음 boundary까지의 데이터가 파일 내용
            size_t nextBoundary = body.find(boundary, pos);
            if (nextBoundary != std::string::npos) {
                fileData = body.substr(pos, nextBoundary - pos - 2);  //마지막  \r\n 제거
                break;  // 파일 데이터를 찾았으므로 반복 종료
            }
        }
        
        pos = lineEnd + 2;  // 다음 줄로 이동
    }
	std::cout<< "&&&&&&&&&&&&&"<<std::endl;
    if (filename.empty() || fileData.empty()) {
        return errorHandler(500);
    }
	std::cout<< "||||||||||||||||"<<std::endl;

	if (access("uploaded", F_OK | W_OK) == -1) {  
		std::cerr << "Directory 'uploaded' does not exist or is not writable" << std::endl;
		return errorHandler(500);
	}

	std::string filepath = "uploaded/uploaded_" + filename;

	while (access(filepath.c_str(), F_OK) != -1) {
		// 파일 확장자와 이름을 분리
		size_t dotPos = filename.find_last_of(".");
		std::string name = filename.substr(0, dotPos);
		std::string ext = filename.substr(dotPos); 
		
		filename = name + "_copy" + ext;
		filepath = "uploaded/uploaded_" + filename;
	}
	std::cout<< "*************"<<std::endl;

	std::ofstream outFile(filepath.c_str(), std::ios::binary);
	if (!outFile.is_open()) {
		std::cerr << "Failed to open file: " << filepath << std::endl;
		return errorHandler(500);
	}
	outFile.write(fileData.c_str(), fileData.length());
	outFile.close();
    
    return textHandler(request, request.getAccept());
}

std::string Response::removeHandler(Request& request)
{
    std::string filepath = request.getMappingUrl();
    
    remove(request.getMappingUrl().c_str());
    return textHandler(request, request.getAccept());
}

std::string Response::executeCgi(const std::vector<std::string>& cgiArgv)
{
    int fd[2];
    pid_t pid;
    if (pipe(fd) == -1)
        return errorHandler(500);
    pid = fork();
    if (pid == -1)
    {
        close (fd[0]);
        close (fd[1]);
        return errorHandler(500);
    }
    if (pid == 0)
    {
        close (fd[0]);
        if (dup2(fd[1], 1) == -1)
        {
            close (fd[1]);
            return errorHandler(500);
        }
        close (fd[1]);
        std::vector<const char*> argv;
        for (size_t i = 0; i < cgiArgv.size(); ++i) {
            argv.push_back(cgiArgv[i].c_str());
        }
        argv.push_back(NULL);
        if (execve(argv[0], (char *const *)&argv[0], NULL) == -1)
            return errorHandler(500);
    }
    close(fd[1]);

    char buffer[4096];
    std::string response;
    ssize_t bytes;

    while ((bytes = read(fd[0], buffer, sizeof(buffer))) > 0)
    {
        response.append(buffer, bytes);
    }
    close(fd[0]);
    if (cgiArgv[0] == "/usr/bin/python3")
    {
        if (response.empty()) {
            return errorHandler(500);
        }
    }

    int status;
    waitpid(pid, &status, 0);

    if (response.find("Content-Length") == std::string::npos) {
        size_t postmp = response.find("\r\n\r\n");
        response.insert(postmp + 2, createContentLength(response));
    }
    return response;
}

std::string Response::cgiHandler(Request& request)
{
    std::vector<std::string> cgiArgv;
    cgiArgv.push_back(request.getLocation().getCgi()[0]);
    cgiArgv.push_back(request.getMappingUrl());

    std::string method = request.getMethod();
    if(method == "GET") getArgv(cgiArgv, request.getQuery());
    else if (method == "POST") getArgv(cgiArgv, request.getBody());
    
	return executeCgi(cgiArgv);
}

std::string Response::RequestHandler(Request& request) {
	if (request.getPath().find(".ico") != std::string::npos) {
		std::string fa = "/home/myeochoi/42webserv/Code/html/image/favi.ico";
		request.setMappingUrl(fa);
		return imageHandler(request, "image/x-icon");
	}
    if (!request.getLocation().getRedirect().empty())
    {
        return redirectHandler(request.getLocation().getRedirect() , "302");
    }
    std::string mapPath = request.getMappingUrl(); 
    if (isDirectory(mapPath)) {
        if (mapPath[mapPath.size() - 1] != '/')
            return redirectHandler("http://" + request.getHeader("Host") + request.getPath() + '/', "301");
    }
	int error = validateRequest(request);
	if (error) return errorHandler(error);


	if (!request.getLocation().getCgi().empty())
	{
		return(cgiHandler(request));
	}
	if (request.getMethod() == "GET") {
		std::string accept = request.getAccept();
		if (accept.find("text") != std::string::npos) {
			return textHandler(request, accept);
		} else if (accept.find("image") != std::string::npos) {
			return imageHandler(request, accept);
		} else {
			return textHandler(request, accept);
		}
	} else if (request.getMethod() == "POST") {
		return postHandler(request);
	} else if (request.getMethod() == "DELETE") {
		return removeHandler(request);

	}
	return 0;
}