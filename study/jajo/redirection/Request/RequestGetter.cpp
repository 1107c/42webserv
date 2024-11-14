#include "Request.hpp"

const std::string& Request::getMethod() const { return this->_method; }
const std::string& Request::getPath() const { return this->_path; }
const std::string& Request::getQuery() const { return this->_query; }
const std::string& Request::getVersion() const { return this->_version; }
const std::string& Request::getBody() const { return this->_body; }
const std::string& Request::getServerName() const { return this->_serverName; }
const std::string& Request::getServerPort() const { return this->_port; }
const std::map<std::string, std::string>& Request::getHeaders() const { return this->_headers; }
std::string Request::getHeader(const std::string& key) const { return this->_headers.at(key); }
