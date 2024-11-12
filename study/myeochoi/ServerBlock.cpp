#include "ServerBlock.hpp"
#include <climits>

ServerBlock::ServerBlock() : _host(""), _port(0), _server(""), _size(0), _error(""), _location()
{

}

ServerBlock::ServerBlock(const ServerBlock &other): _host(other._host), _port(other._port), _server(other._server), _size(other._size), _error(other._error), _location(other._location)
{
}

ServerBlock &ServerBlock::operator&=(const ServerBlock &other)
{
    if (&other != this)
    {
        _host = other._host;
        _port = other._port;
        _server = other._server;
        _size = other._size;
        _error = other._error;
        _location = other._location;
    }
    return *this;
}

ServerBlock::~ServerBlock()
{
}

bool ServerBlock::setHost(const std::string& host)
{
    _host = host;
    return true;
}
bool ServerBlock::setPort(unsigned int port)
{
    if (port > 65535 || port == 0)
        return false;
    _port = port;
    return true;
}
bool ServerBlock::setServerName(const std::string& server) 
{
    _server = server;
    return true;
}
bool ServerBlock::setClientMaxBodySize(unsigned int size) 
{
    if (size == 0 || size > UINT_MAX)
        return false;
    _size = size;
    return true;
}
bool ServerBlock::setErrorPage(const std::string& error)
{
    _error = error;
    return true;
}

void ServerBlock::addLocation(const Location& location) {
    _location.push_back(location);
}

Location& ServerBlock::getLocation() {
    return _location.back();
}