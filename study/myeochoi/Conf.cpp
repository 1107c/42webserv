#include "Conf.hpp"
#include <cstring>
#include <stack>
#include <cctype>


class Conf::InputErrException : public std::exception
{
    private :
        ErrorType _type;
    public :
        InputErrException(ErrorType type) : _type(type) {};
    	const char* what() const throw()
        {
            switch (_type)
            {
                case TOO_MANY_ARGS:
                    return "Too many arguments";
                case FILE_NOT_FOUND:
                    return "File not found";
                case INVALID_FORMAT:
                    return "Invalid format";
                default:
                    return "Unknown error";
            }
        }
};

char* Conf::checkInput(const int &args, char* &argv)
{
    const char *res = "default.conf";
    
    if (args == 1)
        return const_cast<char *>(res);
	else if (args == 2)
        return argv;
	throw Conf::InputErrException(TOO_MANY_ARGS);
}

Conf::Conf(const std::string &file) : _file(file.c_str()), _line("")
{
    if (!_file.is_open())
    {
        throw InputErrException(FILE_NOT_FOUND);
    }
    
    _confMap["host"] = HOST;
    _confMap["listen"] = LISTEN;
    _confMap["server_name"] = SERVER_NAME;
    _confMap["client_max_body_size"] = CLIENT_MAX_BODY_SIZE;
    _confMap["error_page"] = ERROR_PAGE;
    _confMap["location"] = LOCATION;

    _locMap["path"] = PATH;
    _locMap["root"] = ROOT;
    _locMap["index"] = INDEX;
    _locMap["methods"] = METHODS;
    _locMap["autoindex"] = AUTOINDEX;
    _locMap["cgi_path"] = CGI_PATH;
}

Conf::Conf(const Conf &other) : _line(other._line)
{
}

Conf &Conf::operator&=(const Conf &other)
{
    if (&other != this)
    { 
        _line = other._line; 
    }
    return *this;
}

Conf::~Conf()
{

}

void Conf::makeLine()
{
    std::string line;

    while(std::getline(_file, line))
    {
        _line += line + '\n';
    }
    _line.erase(_line.size() - 1);
}

bool Conf::checkBrace()
{
    std::stack<char> checkStack;
    const char* validChars = "{}/_.- ;\n\t";

    for (std::string::const_iterator i = _line.begin(); i != _line.end(); ++i)
    {
        if (!isalnum(*i) && !strchr(validChars, *i))
            return false;
        if (*i == '{')
            checkStack.push('{');
        else if (*i == '}')
        {
            if (checkStack.size() == 0)
                return false;
            checkStack.pop();
        }
    }
    return (checkStack.size() == 0);
}


bool    Conf::isSpace(std::string::const_iterator &i)
{
    return *i == ' ' || *i == '\t' || *i == '\n';
}

bool    Conf::isFirstLine(std::string::const_iterator &i)
{
    const char *server = "server";
    
    if (strncmp(server, &(*i), 6))
        return false;
    i += 6;
    while (*i && isSpace(i))
        ++i;
    return (*i == '{');
}

bool    Conf::allowConf(const std::string &conf)
{
    return _confMap.find(conf) != _confMap.end();
}

bool    Conf::putMethods(const std::string &value, Location& currentLocation, std::string::const_iterator &i)
{
    std::vector<std::string> vec;

    vec.push_back(value);
    while (*i && *i != ';')
    {
        while (*i && isspace(*i)) {++i;}
        std::string::const_iterator start = i; 
        while (*i && !isspace(*i) && *i != ';') {++i;}
        vec.push_back(std::string(start, i));
    }
    return currentLocation.setMethods(vec);
}


bool    Conf::putLocation(const std::string &key, const std::string &value, Location& currentLocation, std::string::const_iterator &i)
{
    // std::cout << value << std::endl;
    switch (_locMap[key])
    {
        case ROOT:
            return currentLocation.setRoot(value);
        case INDEX:
            return currentLocation.setIndex(value);
        case METHODS:
            return putMethods(value, currentLocation, i);
        case AUTOINDEX:
            return currentLocation.setAutoindex(value);
        case CGI_PATH:
            return currentLocation.setCgi(value);
        default :
            return false; 
    }
}

bool    Conf::parseLocation(std::string::const_iterator &i)
{
    ServerBlock& currentBlock = _block.back();
    currentBlock.addLocation(Location());
    Location& currentLocation = currentBlock.getLocation();

    while (*i && !isSpace(i)) {++i;}
    std::string::const_iterator start = i;
    while (*i && !isSpace(i)) {++i;}
    if (!currentLocation.setPath(std::string(start, i)) || !*i)
        return false;
    while (*i && *i != '{') {++i;}
    while (*(++i))
    {
        while (*i && isSpace(i)) {++i;}    
        if (*i == '}')
            return (++i, true);       
        start = i;
        while (*i && !isSpace(i)) {++i;}
        std::string key = std::string(start, i);
        // std::cout <<"key: "<< key << " ";         
        while (*i && isSpace(i)) ++i;
        start = i;
        while (*i && *i != ';' && !isspace(*i)) {++i;}
        if (!putLocation(key, std::string(start, i), currentLocation, i) || *i != ';')
            return false;
    }
    return true;
}

unsigned int Conf::strtoul(const std::string &value)
{
    char *end;

    unsigned long result = std::strtoul(value.c_str(), &end, 10);
    
    if (*end != '\0')
        return 0;
    return result;
}


bool    Conf::putBlock(const std::string &key, const std::string &value)
{
    ServerBlock& currentBlock = _block.back();

    switch (_confMap[key])
    {
        case HOST:
            return currentBlock.setHost(value);
        case LISTEN:
            return currentBlock.setPort(strtoul(value));
        case SERVER_NAME:
            return currentBlock.setServerName(value);
        case CLIENT_MAX_BODY_SIZE:
            return currentBlock.setClientMaxBodySize(strtoul(value));
        case ERROR_PAGE:
            return currentBlock.setErrorPage(value);
        default :
            return false; 
    }
}


bool    Conf::parseLine(std::string::const_iterator &i)
{
    if (*i == '{')
        ++i;
    while (*i && isSpace(i)) {++i;}
    if (*i == '}')
        return true;
    std::string key;
    std::string::const_iterator start = i;

    while (*i && !isSpace(i)) {++i;}
    key = std::string(start, i);
    if (!allowConf(key))
        return false;
    if (key == "location")
    {
        if (!parseLocation(i))
            return false;
        return true;
    }
    while (*i && isSpace(i)) ++i;
    start = i;
    while (*i && *i != ';' && !isspace(*i)) {++i;}
    if (!putBlock(key, std::string(start, i)))
        return false;
    return *(i++) == ';';
}

bool    Conf::parseBlock(std::string::const_iterator &i)
{
    if (!isFirstLine(i))
        return false;
    else
        _block.push_back(ServerBlock());
    while (!isFirstLine(i) && *i)
    {
        if (!parseLine(i))
            return false;
        if (*i)
            ++i;
    }
    if (*i != '\0')
    {
        --i;
        while ((*i == ' ' || *i == '\t' || *i == '\n') && *i)
            --i;
        i -= 6;
    }
    return true;
}



void Conf::parseConf()
{
    makeLine();
    if (!checkBrace())
        throw Conf::InputErrException(INVALID_FORMAT);
    std::string::const_iterator i = _line.begin();

    while (i != _line.end())
    {
        if (!parseBlock(i))
            throw Conf::InputErrException(INVALID_FORMAT);
        if (*i)
            ++i;
    }
}
