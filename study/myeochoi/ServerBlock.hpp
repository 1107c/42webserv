#ifndef SERVERBLOCK_HPP
#define SERVERBLOCK_HPP

#include "Location.hpp"


class ServerBlock
{
    private :
        std::string _host;
        unsigned int _port;
        std::string _server;
        unsigned int _size;
        std::string _error;
        std::vector<Location> _location;

    public :
        ServerBlock();
        ServerBlock(const ServerBlock &other);
        ServerBlock &operator&=(const ServerBlock &other);
        ~ServerBlock();
        bool setHost(const std::string& host);
        bool setPort(unsigned int port);
        bool setServerName(const std::string& server);
        bool setClientMaxBodySize(unsigned int size);
        bool setErrorPage(const std::string& error);
        void addLocation(const Location& location);
        Location& getLocation();
    };

#endif