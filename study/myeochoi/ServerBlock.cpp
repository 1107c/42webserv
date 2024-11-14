#include "ServerBlock.hpp"
#include <climits>
#include <algorithm>
#include <unistd.h> 
ServerBlock::ServerBlock() : _host(""), _port(), _server(), _size(),
 _root(""), _methods(), _autoidx(false), _index(), _error()
{

}

ServerBlock::ServerBlock(const ServerBlock &other): _host(other._host), _port(other._port), _server(other._server), _size(other._size),
 _root(other._root), _methods(other._methods), _autoidx(other._autoidx), _index(other._index), _error(other._error)
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
        _root = other._root;
        _methods = other._methods;
        _autoidx = other._autoidx;
        _index = other._index;
        _error = other._error;
    }
    return *this;
}

ServerBlock::~ServerBlock()
{
}

unsigned int ServerBlock::strtoul(const std::string &value)
{
    char *end;

    unsigned long result = std::strtoul(value.c_str(), &end, 10);
    
    if (*end != '\0')
        return MAX_BODY_SIZE;
    return result;
}
bool ServerBlock::setHost(const std::string& host)
{
    if (host.empty())
        return false;
    int count = 0;
    int dot = 0;
    std::string::const_iterator start = host.begin();
    
    for (std::string::const_iterator i = host.begin(); i != host.end(); ++i)
    {
        if (!isdigit(*i) && *i != '.')
            return false;
        if (*i == '.')
        {
            if ((*(i - 1) && *(i - 1) == '.') || count > 3 
            || (count == 2 && *(i - 2) == '0') || (count == 3 && *(i - 3) == '0')
            || strtoul(std::string(start, i)) > 255)
                return false;
            count = -1;
            ++dot;
            start = i + 1;
        }
        ++count;
    }
    if (dot != 3)
        return false;
    _host = host;
    return true;
}

bool ServerBlock::setPort(unsigned int port)
{
    if (std::find(_port.begin(), _port.end(), port) != _port.end() || port > 65535)
        return false;  
    _port.push_back(port);
    return true;
}
bool ServerBlock::setServerName(const std::string& server) 
{
    if (std::find(_server.begin(), _server.end(), server) != _server.end())
        return false;
    _server.push_back(server);
    return true;
}
bool ServerBlock::setClientMaxBodySize(const std::string &value) 
{
    char *end;

    unsigned long size = std::strtoul(value.c_str(), &end, 10);
    if (*end == 'M')
        size *= 1000 * 1000;
    else if (*end == 'K')
        size *= 1000;
    if (size == 0 || size >= MAX_BODY_SIZE || (*end && (*end != 'M' && *end != 'K')))
        return false;
    _size = size;
    return true;
}

bool ServerBlock::validatePath(const std::string& path)
{
    if (path.empty() || path[0] != '/')
        return false;
    // if (access(path.c_str(), X_OK) != 0)
    //     return false;
   for (size_t i = 0; i < path.length(); i++)
   {
       char c = path[i];
       if ((!isalnum(c) && c != '.' && c != '/' && c != '-' && c != '+') || 
       (i > 1 && c == '/' && path[i - 1] == '/'))
           return false;
   }
   return true;
}


bool ServerBlock::setErrorPage(const std::string& num, const std::string& page)
{    
    unsigned int numInt = strtoul(num);
    
    if (numInt < 400 || numInt > 507 || !validatePath(page))
        return false;
    _error.insert(std::make_pair(num, page));
    return true;
}

bool ServerBlock::setRoot(const std::string& root)
{
    if (    !validatePath(root))
        return false;
    _root = root;
    return true;
}
bool ServerBlock::setIndex(const std::string& index)
{
    if (!_root.empty() && _root[_root.size() - 1] != '/')
        _root += '/';
    // if (access((_root + index).c_str(), X_OK) != 0)
    //     return false;
    _index.push_back(index);
    return true;
}
bool ServerBlock::setMethods(const std::string& methods)
{
    if (!(methods == "GET" || methods == "POST" || methods == "DELETE"))
        return false;
    _methods.push_back(methods);
    return true;
}
bool ServerBlock::setAutoindex(const std::string& autoindex)
{
    if (autoindex != "on" && autoindex != "off")
        return false;
    if (autoindex == "on")
        _autoidx = true;
    return true;
}
const 
std::string &ServerBlock::getHost() const
{
    return _host;
}
const std::vector<unsigned int> &ServerBlock::getPort() const
{
    return _port;
}
const std::vector<std::string> &ServerBlock::getServerName() const
{
    return _server;
}
const unsigned int &ServerBlock::getClientMaxBodySize() const
{
    return _size;
}
const std::map<std::string, std::string> &ServerBlock::getErrorPage() const
{
    return _error;
}
const std::string &ServerBlock::getRoot() const
{
    return _root;
}
const std::vector<std::string> &ServerBlock::getIndex() const
{
    return _index;
}
const std::vector<std::string> &ServerBlock::getMethods() const
{
    return _methods;
}
const bool &ServerBlock::getAutoindex() const
{
    return _autoidx;
}