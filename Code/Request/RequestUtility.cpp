#include "../RequestHeader/Request.hpp"

void Request::setError(int code) {
    this->_errorCode = code;
}

void Request::normalizedPath() { // 경로 정규화 (../와 ./ 처리)
    //1. _path의 마지막 값이 / 인지 아닌지
    if (_location.getRedirect().empty() == false) {
        _mappingUrl = _location.getRedirect();
    }
    else {
        if (_path[_path.length() - 1] == '/') {
            //인덱스가 유효하지 않을 경우 체크
            _mappingUrl = _location.getRoot() + _path + _location.getIndex()[0];
        }
        else if (_path[_path.length() - 1] != '/') {
            _mappingUrl = _location.getRoot() + _path;
        }
    }
}

bool Request::validateRequest() {
    int methodFindIdx = -1;
    for(size_t methodIdx = 0; methodIdx < _location.getMethods().size(); methodIdx++) {
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

