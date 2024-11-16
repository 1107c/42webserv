#include "../ResponseHeader/Response.hpp"

std::string Response::getResponseMessage() const { return (this->_header + this->_body); }