#include "../RequestHeader/Request.hpp"

//생성자
Request::Request() : _contentLength(0)
    , _conf(0)
    , _isParsed(false)
    , _isComplete(false)
    , _errorCode(0) {
}

Request::Request(const std::vector<std::vector<Location> > *conf) :  _contentLength(0)
    ,_conf(conf)
    , _isParsed(false)
    , _isComplete(false)
    , _errorCode(0) {
}

Request::~Request(){
}

bool Request::requestHandler(const std::string& rawRequest) {
	std::cout << "rawRequest: " << rawRequest << "\n";
    if (parse(rawRequest) == false) {

        if (getErrorCode() == 0)
            setError(400);    
        return false;
    }
    if (getConfigOption() == false) {
        return false;
    }
    if (validateRequest() == false) {
        return false;
    }
    _isParsed = true;
    normalizedPath();
    debug();
    return true;
}
