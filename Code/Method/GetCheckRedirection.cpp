#include "../MethodHeader/GetHandler.hpp"

bool GetHandler::checkRedirectionConfig(Request& request) {
    const std::vector<std::vector<Location> > *conf = request.getConfig();
    std::string requestPath = request.getPath();
    int serverBlockIdx = request.getServerBlockIdx();


    for(size_t i = 0; i < (*conf)[serverBlockIdx].size(); i++) {
        if (requestPath == (*conf)[serverBlockIdx][i].getPath())
            return true;
    }
    return false;
}

bool GetHandler::checkRedirectionCondition(Request& request) {
    std::string requestPath = request.getPath();
    if (requestPath.compare(0, 9, "/redirect/", 0, 9) == 0) {
        return true;
    }
    return false;
}

bool GetHandler::checkRedirection(Request& request) {
    //1. 정적 규칙 확인
    if (this->checkRedirectionConfig(request)) {
        return true;
    }

    //2. 조건부 확인
    if (this->checkRedirectionCondition(request)) {
        return true;
    }
    
    return false;
}