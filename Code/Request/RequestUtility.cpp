#include "../RequestHeader/Request.hpp"

void Request::setError(int code) {
    this->_errorCode = code;
}

void Request::normalizedPath() { // 경로 정규화 (../와 ./ 처리)
    //1. _path의 마지막 값이 / 인지 아닌지
    std::cout << "#######\n" << _path << " " << "$$$$$$$$$$$\n" << _location.getRoot() << std::endl;
    if (_location.getRedirect().empty() == false) {
        _mappingUrl = _location.getRedirect();
    }
    else {
        _mappingUrl = setNormalizedPath(_path, _location.getRoot());
    }
}

bool Request::validateRequest() {
    int methodFindIdx = -1;
    std::cout <<"size :"<< _location.getRoot() <<std::endl;
    for(size_t methodIdx = 0; methodIdx < _location.getMethods().size(); methodIdx++) {
        std::cout <<"method :"<<this->getMethod()<<" :confmethod :"<<_location.getMethods()[methodIdx] <<std::endl;
        if (this->getMethod() == _location.getMethods()[methodIdx]) {
            methodFindIdx = methodIdx;
        }
    }
    if (methodFindIdx == -1) {
        setError(405);
        return false;
    }

    if (this->_contentLength > _location.getClientMaxBodySize()) {
        setError(400);
        return false;
    }
    return true;
}

void Request::setMappingUrl(std::string& path) {
    this->_mappingUrl = path;
}

void Request::setBody(const std::string& body) {
    this->_body = body;
}