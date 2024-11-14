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
