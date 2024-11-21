#include "../Socket/Epoll.hpp"
#include "../UtilityHeader/Utility.hpp"

std::string Response::errorHandler(int error) {
	std::string path = getErrorPath(error);
	std::string header = getErrorHeader(error);
	std::ifstream file(path.c_str());
	if (!file.is_open()) header = getErrorHeader(500);
	std::string html;
	std::string line;
	while (std::getline(file, line)) {
		html += line + "\n";
	}
	std::ostringstream ss;
	ss << html.size();

	std::string response = header + "\r\n";
	response += "Content-Type: text/html; charset=UTF-8\r\n";
	response += "Content-Length: " + ss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Date: " + getGMTDate() + "\r\n\r\n";
	response += html;
	return response;
}

std::string Response::textHandler(const Request& request, const std::string& accept) {
	std::string path = request.getMappingUrl();
	if (isDirectory(path)) {
		// path =                                << cgi를 이용해서 autoindex 처리할것
	}
	std::ifstream file(request.getMappingUrl().c_str());
	if (!file.is_open()) return errorHandler(500);
    std::string html;
    std::string line;
    while (std::getline(file, line)) {
        html += line + "\n";
    }
    std::ostringstream ss;
    ss << html.size();
    
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: " + accept + "\r\n";
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
    
    // Content-Type 헤더에서 boundary 추출

    // size_t boundaryPos = request.getHeader("Content-Type").find("boundary=");
    // if (boundaryPos != std::string::npos) {
    //     boundary = request.getHeader("Content-Type").substr(boundaryPos + 9);
    // }
    // if (boundary.empty()) {
    //     return NULL;
    // }
    size_t pos = 0;
    std::string filename;
    std::string contentType;
    std::string fileData;

    // 첫 번째 boundary 찾기

	// boundaryPos = body.find("boundary=");
    pos = body.find("\r\n", pos);
	// std::cout << body << std::endl;
    if (pos == std::string::npos)
	{		
    	std::cout<< "@@@@@@"<<std::endl;
		return NULL;
	} 
	boundary = body.substr(0, pos - 2);
	std::cout <<boundary <<std::endl;
    pos = pos + 2;  
    	std::cout<< "@@@@@@"<<std::endl;
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
        return NULL;
    }
	std::cout<< "||||||||||||||||"<<std::endl;

	if (access("uploaded", F_OK | W_OK) == -1) {  
		std::cerr << "Directory 'uploaded' does not exist or is not writable" << std::endl;
		return NULL;
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
		return NULL;
	}
	outFile.write(fileData.c_str(), fileData.length());
	outFile.close();
    
    // std::cout << "Successfully saved file: " << filepath << std::endl;
    return textHandler(request, request.getAccept());
}

std::string Response::removeHandler(Request& request)
{
    std::string filepath = request.getMappingUrl();
    


    remove(request.getMappingUrl().c_str());
	// std::cout <<"path" << filepath << std::endl;
	// filepath= '.' + filepath;
    // int fd = open(filepath.c_str(), O_WRONLY | O_TRUNC);
    // if (fd == -1) {
	// 	std::cout << "@@@@@"<<std::endl;
    //     return NULL;
    // }

    // 파일을 닫음 (이미 O_TRUNC로 내용이 비워짐)
    // close(fd);
    return textHandler(request, request.getAccept());
}

std::string Response::cgiHandler(const Location& location,const  std::string &url)
{
	std::cout << location.getCgi()[0] << std::endl;
	std::string execZero =  location.getCgi()[0].c_str();
	std::string tmp = url;
	if (url[url.size() - 1] != '/')
		tmp += '/';
	std::string execOne = tmp + location.getIndex()[0];

	const char *args[3] =
	{
		execZero.c_str(),
		execOne.c_str(),
		NULL
	};  
	return executeCgi(args);
}


std::string Response::RequestHandler(Request& request) {
	if (request.getPath().find(".ico") != std::string::npos) {
		std::string fa = "/home/myeochoi/42webserv/Code/html/image/favi.ico";
		request.setMappingUrl(fa);
		return imageHandler(request, "image/x-icon");
	}
		// std::cout <<"@@@@@@@@@@@\n" << request.getLocation().getRedirect() << std::endl;
	// int error = validateRequest(request);
	// if (error) return errorHandler(error);
	if (request.getLocation().getAutoindex() == true)
    {
        return (autoIndexHandler(request));
    }
    if (!request.getLocation().getCgi().empty())
	{
		return(cgiHandler(request.getLocation(), request.getMappingUrl()));
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
		// std::string type = request.getContentType();
		// if (type == "application/x-www-form-urlencoded") {

		// } else if (type == "application/json") {

		// } else if (type == "multipart/form-data") {


		// } else if (type == "application/octet-stream") {
		// } else if (type == "text/plain") {

		// }
	} else if (request.getMethod() == "DELETE") {
		return removeHandler(request);

	}
	return 0;
}


std::string Response::executeCgi(const char *(&args)[3])
{
   int fd[2];
   pid_t pid;
   
   if (pipe(fd) == -1)
       throw std::runtime_error("Failed to pipe");
   pid = fork();
   if (pid == -1)
   {
       close (fd[0]);
       close (fd[1]);
       throw std::runtime_error("Failed to fork");
   }
   if (pid == 0)
   {
       close (fd[0]);
       if (dup2(fd[1], 1) == -1)
       {
           close (fd[1]);
           throw std::runtime_error("Failed to fork");
       }
       close (fd[1]);
       if (execve(args[0], (char *const *)args, NULL) == -1)
           throw std::runtime_error("Failed to exec");
   }
   close(fd[1]);

   char buffer[4096];
   std::string response;
   std::string headers;
   std::string body;
   bool isHeader = false;
   ssize_t bytes;
   
   while ((bytes = read(fd[0], buffer, sizeof(buffer))) > 0)
   {
       response.append(buffer, bytes);
       if (!isHeader)
       {
           size_t header_end = response.find("\r\n\r\n");
           if (header_end != std::string::npos)
           {
               headers = response.substr(0, header_end);
               body = response.substr(header_end + 4);
               isHeader = true;
           }
       }
   }
   close(fd[0]);
   
   int status;
   waitpid(pid, &status, 0);

   bool hasContentLength = false;
   std::stringstream fullResponse;
   fullResponse << "HTTP/1.1 200 OK\r\n";
   
   if (isHeader)
   {
       std::istringstream header_stream(headers);
       std::string line;
       while (std::getline(header_stream, line))
       {
           if (line.find("Content-Length:") != std::string::npos)
           {
               hasContentLength = true;
               fullResponse << line << "\r\n";
           }
           else if (line.find("Content-Type:") != std::string::npos)
               fullResponse << line << "\r\n";
       }
   }
   
   if (!isHeader || headers.find("Content-Type:") == std::string::npos)
   {
       if (!strcmp("/usr/bin/python3", args[0]))
           fullResponse << "Content-Type: text/html\r\n";
       else
           fullResponse << "Content-Type: image/jpeg\r\n";
   }
   
   if (!hasContentLength)
   {
       if (!isHeader) 
           body = response;
       fullResponse << "Content-Length: " << body.length() << "\r\n\r\n";
       fullResponse << body;
   }
   std::cout << "@@@@\n"<<fullResponse.str()<< "\n @@@@"	;
   return fullResponse.str();
}