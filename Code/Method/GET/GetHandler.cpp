#include "../../MethodHeader/GetHandler.hpp"

GetHandler::GetHandler() {}

bool GetHandler::handleRequest(Request &requestMessage) {
    //1. 리다이렉션 확인
    if (checkRedirection(requestMessage)) {
        std::cout << "Redirection 요청\n";
        this->_isRedirection = true;
        return true;
    }
    //2. 데이터 요청 확인
    else {
        std::cout << "파일 정보 요청\n";
        //1. 존재 여부 확인 404
        //2. 접근 여부 확인 403
        if (checkFileAccess(requestMessage)) {
            this->_isRedirection = false;
            return true;
        }
    }
    return false;
}