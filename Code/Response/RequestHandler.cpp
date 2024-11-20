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

std::string Response::RequestHandler(Request& request) {
	if (request.getPath().find(".ico") != std::string::npos) {
		std::string fa = "/home/changhyu/st/cursus5/42webserv/Code/html/image/favi.ico";
		request.setMappingUrl(fa);
		return imageHandler(request, "image/x-icon");
	}
	int error = validateRequest(request);
	if (error) return errorHandler(error);
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
		std::string type = request.getContentType();
		if (type == "application/x-www-form-urlencoded") {

		} else if (type == "application/json") {

		} else if (type == "multipart/form-data") {

		} else if (type == "application/octet-stream") {

		} else if (type == "text/plain") {

		}
	} else if (request.getMethod() == "DELETE") {

	}
	return 0;
}
