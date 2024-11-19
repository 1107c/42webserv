#include "../ResponseHeader/Response.hpp"

bool Response::getRedirectionFile(Request& request) {
    std::cout << "파일 읽고 리스폰스 남기미\n";
    std::cout << "path : " << request.getPath() << std::endl;
    std::string filePath = request.getMappingUrl();
    std::ifstream inputFile(filePath.c_str());


    if (!inputFile.is_open()) {
        return false;
    }

    std::string fileContent;
    std::string line;
    while(std::getline(inputFile, line)) {
        fileContent += line + "\r\n";
    }
    inputFile.close();
    this->_body = fileContent;
    _contentLength = fileContent.length();
    return true;
}

bool Response::getErrorFile(Request& request, int statusCode) {
    std::string filePath = "/home/ksuh/webserv/Code/ErrorHtml/";
    // std::string filePath = "/home/jajo/webserv/Code/ErrorHtml/";
    
    std::map<std::string, std::string> map = (*request.getConfig())[request.getServerBlockIdx()][0].getErrorPage();
    std::map<std::string, std::string>::iterator it = map.find(ToString(statusCode));
    if (it != map.end()) {
        filePath += it->second;
    } else {
        filePath += ToString(statusCode) + ".html";
    }
    std::cout << "error url : " << filePath << std::endl;
    std::ifstream inputFile(filePath.c_str());
    if (!inputFile.is_open()) {
        return false;
    }

    std::string fileContent;
    std::string line;
    while(std::getline(inputFile, line)) {
        fileContent += line + "\r\n";
    }
    inputFile.close();
    this->_body = fileContent;
    _contentLength = fileContent.length();
    return true;
}

bool Response::getFaviconFile(Request& request) {
    (void)request;
    // std::string path = "/home/jajo/redirection/request/favicon.ico";
    std::string path = "/home/ksuh/redirection/favicon.ico";
    std::ifstream file(path.c_str(), std::ios::binary);

    if (!file.is_open()) {
        return false;
    }

    // 2. 파일 크기 얻기
    file.seekg(0, std::ios::end); // 파일 끝으로 이동
    std::streamsize file_size = file.tellg(); // 현재 위치로 파일 크기 확인
    file.seekg(0, std::ios::beg); // 다시 파일 시작으로 이동

    std::string fileContent;
    std::string line;
    while(std::getline(file, line)) {
        fileContent += line + "\r\n";
    }
    file.close();
    this->_body = fileContent;
    this->_contentLength = file_size;
    return true;
}
