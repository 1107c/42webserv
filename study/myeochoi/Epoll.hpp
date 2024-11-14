#ifndef EPOLL_HPP
#define EPOLL_HPP

#include "Conf.hpp"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h> 

class Epoll 
{
    private :
        const std::vector<std::vector<Location> >& _config;
        int _epollfd;
        std::vector<int> _socket; 
    public :
        Epoll(const Conf& config);
        Epoll(const Epoll &other);
        Epoll &operator&=(const Epoll &other);
        ~Epoll();
        void run();
        void initSocket();
        void bindSocket(int &socket, const Location &loc, std::vector<std::string> &ip);
};

#endif