#include "../ResponseHeader/Response.hpp"

//1. 에러 메시지


//2. Get 리다이렉션 메시지
void Response::makeResponseRedirectionMessage(Request& request) {
    unsigned long long fl = getRedirectionFileLength(request);
    std::ostringstream oss;
    oss << fl;
    std::string str = oss.str();

    _header += request.getVersion() + " 301 Moved Permanently\r\n";
    _header += "Location: " + request.getNormalizedPath() + "\r\n";
    _header += "Content-Type: text/html\r\n";
    _header += "Content-Length: " + str + "\r\n";
    _header += "\r\n";

    _body += getRedirectionFileContent(request) + "\r\n";
}
//3. Get 데이터 요청 메시지
//4. POST 메시지
//5. Delete 메시지
