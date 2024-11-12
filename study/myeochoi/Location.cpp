#include "Location.hpp"

Location::Location(): _path(""), _root(""), _index(""), _methods(), _autoindex(false), _cgi("")
{

}

Location::Location(const Location &other): _path(other._path), _root(other._root), _index(other._index), _methods(other._methods), _autoindex(other._autoindex), _cgi(other._cgi)
{

}

Location& Location::operator&=(const Location &other)
{
    if (&other != this)
    {
        _path = other._path;
        _root = other._root;
        _index = other._index;
        _methods = other._methods;
        _autoindex = other._autoindex;
        _cgi = other._cgi;
    }
    return *this;
}

Location::~Location()
{

}


bool Location::setPath(const std::string& path)
{
    _path = path;
    return true;
}
bool Location::setRoot(const std::string& root)
{
    _root = root;
    return true;
}
bool Location::setIndex(const std::string& index)
{
    _index = index;
    return true;
}
bool Location::setMethods(const std::vector<std::string>& methods)
{
    _methods = methods;
    return true;
}
bool Location::setAutoindex(const std::string& autoindex)
{
    if (autoindex != "on" && autoindex != "off")
        return false;
    if (autoindex == "on")
        _index = true;
    return true;
}
bool Location::setCgi(const std::string& cgi)
{
    _cgi = cgi;
    return true;
}