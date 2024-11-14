#include "Epoll.hpp"
#include <algorithm>

Epoll::Epoll(const Conf& config) : _config(config.getServerBlocks()), _epollfd(0), _socket()
{
}

Epoll::Epoll(const Epoll &other) : _config(other._config), _epollfd(other._epollfd), _socket(other._socket)
{
}

Epoll &Epoll::operator&=(const Epoll &other)
{
    if (&other != this)
    {
        _epollfd = other._epollfd;
        _socket = other._socket;
    }
    return *this;
}

Epoll::~Epoll()
{

}

void Epoll::run()
{
    _epollfd = epoll_create(42);
    initSocket();

    close(_epollfd);
}

void Epoll::bindSocket(int &socket, const Location &loc, std::vector<std::string> &ip)
{
    if (ip.empty() || std::find(ip.begin(), ip.end(), loc.getHost()) == ip.end())
    {
        struct sockaddr_in addr;
        struct addrinfo hints;
        struct addrinfo* result;
        // _socket.push_back(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0));
        // int socket = _socket.back(); 
        // if (socket == -1)
        //     throw std::runtime_error("server socket create failed");
        int opt = 1;
        if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
            throw std::runtime_error("setsockopt SO_REUSEADDR failed");
        ip.push_back(loc.getHost());
        memset(&addr, 0, sizeof(addr));
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

       int status = getaddrinfo(loc.getHost().c_str(), NULL, &hints, &result);
        if (status != 0)
            throw std::runtime_error("getaddrinfo failed");
        addr.sin_addr = ((struct sockaddr_in *)(result->ai_addr))->sin_addr;
        freeaddrinfo(result);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        if (bind(socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)
            throw std::runtime_error("socket bind failed");
        if (listen(socket, SOMAXCONN) == -1)
            throw std::runtime_error("socket listen failed");
    }
}

void Epoll::initSocket()
{
    if (_epollfd == -1)
        throw std::runtime_error("epoll create failed");
    
    std::map<std::string, unsigned int> checkport;
    for (unsigned int i = 0; i != _config.size(); ++i)
    {
        for (unsigned int j = 0; j != _config[i].size(); ++j)
        {
            if (checkport.empty())
            {
                for (unsigned int k = 0; k != _config[i][j].getPort()[k]; ++k)
                    checkport.insert(make_pair(_config[i][j].getHost(), _config[i][j].getPort()[k]));
            }
            //    bindSocket(socket, _config[i][j], checkIp);
        }
    }
}