#include "../MethodHeader/GetHandler.hpp"

GetHandler::GetHandler() {}

bool GetHandler::handleRequest(Request &requestMessage) {
    //1. 리다이렉션 확인
    if (checkRedirection(requestMessage)) {
        std::cout << "Request 맞음!!!!!!!!!!!!!!!!\n";
        return true;
    }
    //2. 데이터 요청 확인
    else {
        std::cout << "Request 아님!!!!!!!!!!!!!!!!!!\n";
        return false;
    }
    return false;
}