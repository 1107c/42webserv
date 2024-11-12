
std::string makeRedirectResponse(const std::string& newLocation) {
    std::string response;
    
    response = "HTTP/1.1 301 Moved Permanently\r\n";
    response += "Location: " + newLocation + "\r\n";
    response += "Content-Length: 0\r\n";
    response += "\r\n";
    
    return response;
}

void processRequest(int clientSock, const Request& request) {
    try {
        //1. 설정 확인
        LocationiConfig location = getLocationConfig(request.getPath());

        //2. 리다이렉션 필요 여부 확인
        if (location.hasRedirect()) {
            auto [code, url] = location.getRedirectInfo();

            //3. 리다이렉션 응답 생성
            std::string response = "HTTP/1.1 " + std::to_string(code) + " Moved\r\n";
            response += "Location: " + url + "\r\n";
            response += "Content-Length: 0\r\n\r\n";

            //4. 응답 전송
            send(clientSock, response.c_str(), response.length(), 0);
            return ;
        }
    } catch (const std::exception& e) {
        sendErrorResponse(clientSock, 500);
    }
}

void sendRedirectResponse(int clientSock, const std::string& newLocation) {
    std::string response = makeRedirectResponse(newLocation);

    ssize_t bytesent = send(clientSock, response.c_str(), response.length(), 0);
    if (bytesent < 0) {
        throw std::runtime_error("Failed to send redirect response");
    }
}

void handleRequest(const Request& request) {
    try {
        processRequest(request);
    } catch (const RedirectionException& e) {
        sendRedirectResponse(clientSock, e.getCode, e.getLocation);
    } catch (const std::exception& e) {
        sendErrorResponse(clientSock, 500);
    }
}

