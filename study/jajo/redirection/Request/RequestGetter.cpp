#include "Request.hpp"

const std::string& Request::getMethod() const { return this->_method; }
const std::string& Request::getPath() const { return this->_path; }
const std::string& Request::getQuery() const { return this->_query; }
const std::string& Request::getVersion() const { return this->_version; }
const std::string& Request::getBody() const { return this->_body; }
const std::map<std::string, std::string>& Request::getHeaders() const { return this->_headers; }
std::string Request::getHeader(const std::string& key) const { return this->_headers.at(key); }
