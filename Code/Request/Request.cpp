#include "../RequestHeader/Request.hpp"

//생성자
Request::Request() : _conf(0)
    , _isParsed(false)
    , _isComplete(false)
    , _contentLength(0)
    , _errorCode(0) {
}

Request::Request(const std::vector<std::vector<Location> > *conf) :  _conf(conf)
    , _isParsed(false)
    , _isComplete(false)
    , _contentLength(0)
    , _errorCode(0) {
}

Request::~Request(){
}

bool Request::requestHandler(const std::string& rawRequest) {
    if (parse(rawRequest) == false) {
        if (getErrorCode() == 0)
            setError(400);
        return false;
    }
    debug();
    return true;
}
