#include "Request.hpp"

//생성자
Request::Request() : _isParsed(false)
    , _isComplete(false)
    , _contentLength(0)
    , _errorCode(0) {
}

Request::~Request(){
}
