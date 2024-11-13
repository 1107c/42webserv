#include "Location.hpp"

Location::Location(): ServerBlock(), _path(""), _cgi()
{
}

Location::Location(const Location &other) : ServerBlock(other), _path(other._path), _cgi(other._cgi)
{
    
}

Location& Location::operator&=(const Location &other) 
{
    if (&other != this)
    {
        ServerBlock::operator&=(other);
        _path = other._path;
        _cgi = other._cgi;
    }
    return *this;
}

Location::~Location()
{

}


bool Location::setPath(const std::string& path)
{
    if (!validatePath(path))
        return false;
    _path = path;
    return true;
}

bool Location::setCgi(const std::string& cgi)
{
    if (!validatePath(cgi))
        return false;
    _cgi.push_back(cgi);
    return true;
}

const std::string &Location::getPath() const
{
    return _path;
}
const std::vector<std::string> &Location::getCgi() const
{
    return _cgi;
}