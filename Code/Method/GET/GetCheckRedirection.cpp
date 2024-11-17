#include "../../MethodHeader/GetHandler.hpp"

bool GetHandler::checkRedirectionConfig(Request& request) {
    const std::vector<std::vector<Location> > *conf = request.getConfig();
    std::string requestPath = request.getPath();
    int serverBlockIdx = request.getServerBlockIdx();


    std::cout << "config path : ";
    for(size_t i = 0; i < (*conf)[serverBlockIdx].size(); i++) {
        std::cout << (*conf)[serverBlockIdx][i].getPath() << " ";
        if (requestPath == (*conf)[serverBlockIdx][i].getPath())
            return true;
    }
    std::cout << std::endl;
    return false;
}

bool GetHandler::checkRedirectionCondition(Request& request) {
    std::string requestPath = request.getPath();
    std::cout << "config path\n";
    std::cout << request.getPath() << std::endl;
    if (requestPath.compare(0, 12, "/redirection/", 0, 12) == 0) {
        return true;
    }
    return false;
}

bool GetHandler::checkRedirection(Request& request) {
    //1. 정적 규칙 확인
    if (this->checkRedirectionConfig(request)) {
        std::cout << "config 파일 내에서 리다이렉션 확인\n";
        return true;
    }

    //2. 조건부 확인
    if (this->checkRedirectionCondition(request)) {
        std::cout << "패턴 내에서 리다이렉션 확인\n";
        return true;
    }
    
    return false;
}