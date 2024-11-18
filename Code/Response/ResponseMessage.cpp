#include "../ResponseHeader/Response.hpp"

//1. Get 리다이렉션 메시지
void Response::makeResponseRedirectionMessage(Request& request) {
    std::cout << "리다이렉션 리스폰스 메시지\n";
    this->_header.erase(0, std::string::npos);
    this->_body.erase(0, std::string::npos);

    _header += request.getVersion() + " 301 Moved Permanently\r\n";
    _header += "Location: " + request.getMappingUrl() + "\r\n";
    _header += "Content-Type: " + request.getAccept() + "\r\n";
    _header += "Content-Length: 0\r\n";
    _header += "\r\n";
}

//2. Get 데이터 요청 메시지
void Response::makeResponseGetMessage(Request& request) {
    std::cout << "겟 리스폰스 메시지\n";
    this->_header.erase(0, std::string::npos);
    this->_body.erase(0, std::string::npos);

    if (request.getPath() == "/favicon.ico") {
        if (getFaviconFile(request) == false) {
            makeErrorMessage(request, 500);
            return ;
        }
        _body += "\r\n";
        _header += request.getVersion() + " 200 OK\r\n";
        _header += "Content-Type: " + request.getAccept() + "\r\n";
        _header += "Content-Length: " + ToString(_contentLength) + "\r\n";
        _header += "Cache-Control: public, max-age=86400\r\n";
        _header += "\r\n";

    } else {
        if (getRedirectionFile(request) == false) {
            makeErrorMessage(request, 500);
            std::cerr << "뭔가 문제\n";
            return ;
        }
        _body += "\r\n";
        _header += request.getVersion() + " 200 OK\r\n";
        _header += "Content-Type: " + request.getAccept() + "\r\n";
        _header += "Content-Length: " + ToString(_contentLength) + "\r\n";
        _header += "\r\n";
    }
}

//3. POST 메시지
//4. Delete 메시지

//5. 에러 메시지
void Response::makeErrorMessage(Request& request, int statusCode) {
    this->_header.erase(0, std::string::npos);
    this->_body.erase(0, std::string::npos);

    if (getErrorFile(request, statusCode) == false) {
        statusCode = 500;
    }
    _body += "\r\n";
    
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

    this->_header = statusLine;
    this->_header += "Content-Type: text/html\r\n";
    this->_header += "Content-Length: " + ToString(_contentLength) + "\r\n";
    this->_header += "\r\n";
    //오류 파일 맵핑해서 같이 보내기
}