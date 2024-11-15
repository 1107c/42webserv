#include "../ResponseHeader/Response.hpp"

std::string Response::getRedirectionFileContentLength(Request& request) {
    std::string filePath = request.getPath();
    std::ifstream inputFile(filePath.c_str());

    if (!inputFile.is_open()) {
        // request.setError(400);
        // return "error";
        std::cout << "겟 파일 읽기 실패\n";
    }

    std::string fileContent;
    std::string line;
    while(std::getline(inputFile, line)) {
        fileContent += line + "\r\n";
    }
    inputFile.close();
    this->_body = fileContent;
    _contentLength = fileContent.length();
    return fileContent;
}

std::string Response::getErrorFileContentLength(int statusCode) {
    std::string filePath = "/home/jajo/webserv/Code/ErrorHtml/";
    
    std::stringstream ss;
    ss << statusCode;
    std::string str = ss.str();

    filePath += str + ".html";

    std::ifstream inputFile(filePath.c_str());
    if (!inputFile.is_open()) {
        // request.setError(400);
        // return "error";
        std::cout << "에러파일 읽기 실패\n";
    }

    std::string fileContent;
    std::string line;
    while(std::getline(inputFile, line)) {
        fileContent += line + "\r\n";
    }
    inputFile.close();
    this->_body = fileContent;
    _contentLength = fileContent.length();
    return fileContent;
}
