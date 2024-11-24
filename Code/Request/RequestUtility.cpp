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
        std::string path;
        _isAutoindex = true;
        

        for (size_t i = 0; i < _location.getIndex().size(); ++i)
        {
            std::string temp;
            path = "";
            if (_path == _location.getPath()) {
                if (!_location.getCgi().empty())
                    path = _path + '/' + _location.getIndex()[i];
                else
                    path = _path + _location.getIndex()[i];
            }
            else
                path = _path;
            for (size_t i = 0, j = 0; i < path.size(); ++i) {
                if (i && (temp[j] == '/' && path[i] == '/')) continue;
                temp += path[i];
                j++;
            }
            path = _location.getRoot() + temp;

            if (!checkPermissions(path))
            {
                _isAutoindex = false;
                _myIndex = _path;
                _path = temp;
                break;
            }
        }
        _mappingUrl = path; // 아무것도 존재하지 않을 경우 뭘줘야하지..?



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