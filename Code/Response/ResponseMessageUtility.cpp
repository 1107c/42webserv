#include "../ResponseHeader/Response.hpp"

unsigned long long Response::getRedirectionFileLength(Request& request) {
    std::string filePath = request.getNormalizedPath();
    std::ifstream inputFile(filePath.c_str());

    if (!inputFile.is_open()) {
        //request.setError(400);
    }

    std::string fileContent;
    std::string line;
    while(std::getline(inputFile, line)) {
        fileContent += line;
    }
    inputFile.close();
    return fileContent.length();
}

std::string Response::getRedirectionFileContent(Request& request) {
    std::string filePath = request.getNormalizedPath();
    std::ifstream inputFile(filePath.c_str());

    if (!inputFile.is_open()) {
        // request.setError(400);
        // return "error";
    }

    std::string fileContent;
    std::string line;
    while(std::getline(inputFile, line)) {
        fileContent += line;
    }
    inputFile.close();
    this->_body = fileContent;
    return fileContent;
}
