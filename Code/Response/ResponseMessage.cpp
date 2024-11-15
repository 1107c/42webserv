#include "../ResponseHeader/Response.hpp"

//1. 에러 메시지


//2. Get 리다이렉션 메시지
void Response::makeResponseRedirectionMessage(Request& request) {
    this->_header.erase(0, std::string::npos);
    this->_body.erase(0, std::string::npos);

    _header += request.getVersion() + " 301 Moved Permanently\r\n";
    _header += "Location: " + request.getMappingUrl() + "\r\n";
    _header += "Content-Type: text/html\r\n";
    _header += "Content-Length: 0\r\n";
    _header += "\r\n";

    //_body += getRedirectionFileContent(request) + "\r\n";
}
//3. Get 데이터 요청 메시지
void Response::makeResponseGetMessage(Request& request) {
    this->_header.erase(0, std::string::npos);
    this->_body.erase(0, std::string::npos);


    _body += getRedirectionFileContentLength(request) + "\r\n";

    std::ostringstream oss;
    oss << _contentLength;
    std::string str = oss.str();

    _header += request.getVersion() + " 200 OK\r\n";
    _header += "Content-Type: text/html\r\n";
    _header += "Content-Length: " + str + "\r\n";
    _header += "\r\n";
}

//4. POST 메시지
//5. Delete 메시지

//6. 에러 메시지
void Response::makeErrorMessage(int statusCode) {
    this->_header.erase(0, std::string::npos);
    this->_body.erase(0, std::string::npos);

    _body += getErrorFileContentLength(statusCode) + "\r\n";
    
    std::string statusLine;
    switch (statusCode) {
        case 400:
            statusLine = "HTTP/1.1 400 Bad Request\r\n";
            break;
        case 401:
            statusLine = "HTTP/1.1 401 Unauthorized\r\n";
            break;
        case 403:
            statusLine = "HTTP/1.1 403 Forbidden\r\n";
            break;
        case 404:
            statusLine = "HTTP/1.1 404 Not Found\r\n";
            break;
        case 405:
            statusLine = "HTTP/1.1 405 Method Not Allowed\r\n";
            break;
        case 500:
            statusLine = "HTTP/1.1 500 Internal Server Error\r\n";
            break;
        case 501:
            statusLine = "HTTP/1.1 501 Not Implemented\r\n";
            break;
        default:
            statusLine = "HTTP/1.1 500 Internal Server Error\r\n";
    }

    std::ostringstream oss;
    oss << _contentLength;
    std::string str = oss.str();

    this->_header = statusLine;
    this->_header += "Content-Type: text/html\r\n";
    this->_header += "Content-Length: " + str + "\r\n";
    this->_header += "\r\n";
    //오류 파일 맵핑해서 같이 보내기
}