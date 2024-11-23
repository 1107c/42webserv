#include "../RequestHeader/Request.hpp"

void Request::setError(int code) {
    this->_errorCode = code;
}

void Request::normalizedPath() { // 경로 정규화 (../와 ./ 처리)
    //1. _path의 마지막 값이 / 인지 아닌지
    // if (_location.getRedirect().empty() == false) {
    //     _mappingUrl = _location.getRedirect();
    // }
    {
        std::string temp;

		if (!_location.getCgi().empty())
			_path = _path + '/' + _location.getIndex()[0];
		else if (_path == "/")
			_path = _path + _location.getIndex()[0];

        for (size_t i = 0, j = 0; i < _path.size(); ++i) {
            if (i && (temp[j] == '/' && _path[i] == '/')) continue;
            temp += _path[i];
            j++;
        }
        _mappingUrl = _location.getRoot() + temp;

		// std::cout << "\n\n====================TEST========================\n\n";
		// std::cout << _path << "\n";
		// std::cout << _location.getRoot() << "\n";
		// std::cout << _location.getPath() << "\n";
		// if (!_location.getCgi().empty())
		// 	std::cout << _location.getCgi()[0] << "\n";
		// if (!_location.getIndex().empty())
		// 	std::cout << _location.getIndex()[0] << "\n";
		// std::cout << "\n\n================================================\n\n";
    }
}

bool Request::validateRequest() {
    int methodFindIdx = -1;
    // std::cout <<"size :"<< _location.getRoot() <<std::endl;
    for(size_t methodIdx = 0; methodIdx < _location.getMethods().size(); methodIdx++) {
        // std::cout <<"method :"<<this->getMethod()<<" :confmethod :"<<_location.getMethods()[methodIdx] <<std::endl;
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

void Request::printHeaders(void) {
	std::map<std::string, std::string>::iterator it;

	std::cout << "\n\n=========Request Handler Headers=========\n\n";
	for (it=_headers.begin(); it!=_headers.end(); it++)
	{
		std::cout << it->first << ": " << it->second << "\n";
	}
	std::cout << "\n\n=========================================\n\n";
}