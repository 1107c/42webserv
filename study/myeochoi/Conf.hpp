#ifndef CONF_HPP
#define CONF_HPP

#include "ServerBlock.hpp"
#include <map>

enum ErrorType
{
    TOO_MANY_ARGS,
    FILE_NOT_FOUND,
    INVALID_FORMAT,
};

enum ConfigType {
    HOST,
    LISTEN,
    SERVER_NAME,
    CLIENT_MAX_BODY_SIZE,
    ERROR_PAGE,
    LOCATION,
};

enum LocationType {
    PATH,
    ROOT,
    INDEX,
    METHODS,
    AUTOINDEX,
    CGI_PATH,
};

class Conf
{
    private :
        std::ifstream _file;
        std::string _line;
        std::vector<ServerBlock> _block;
        std::map<std::string, ConfigType> _confMap;
        std::map<std::string, LocationType> _locMap;
    public :
        class InputErrException;
        static char* checkInput(const int &args, char* &argv);
        Conf(const std::string &file);
        Conf(const Conf &other);
        Conf &operator&=(const Conf &other);
        ~Conf();
        void    parseConf();
        void    makeLine();
        bool    checkBrace();
        bool    parseBlock(std::string::const_iterator &i);
        bool    parseLine(std::string::const_iterator &i);
        bool    putBlock(const std::string &key, const std::string &value);
        bool    parseLocation(std::string::const_iterator &i);
        bool    putLocation(const std::string &key, const std::string &value, Location& currentLocation, std::string::const_iterator &i);    
        bool    putMethods(const std::string &value, Location& currentLocation, std::string::const_iterator &i);    
        bool    allowConf(const std::string &conf);
        bool    isFirstLine(std::string::const_iterator &i);
        bool    isSpace(std::string::const_iterator &i);
        unsigned int strtoul(const std::string &value);


};

#endif