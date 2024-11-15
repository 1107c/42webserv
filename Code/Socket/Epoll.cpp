#include "Epoll.hpp"


Epoll::Epoll(const Conf& config) : _config(config.getServerBlocks()), _epollfd(0), _socket(), _result(), _pendingResponses()
{
}

Epoll::Epoll(const Epoll &other) : _config(other._config), _epollfd(other._epollfd), _socket(other._socket), _result(other._result), _pendingResponses(other._pendingResponses)
{
}

Epoll &Epoll::operator&=(const Epoll &other)
{
    if (&other != this)
    {
        _epollfd = other._epollfd;
        _socket = other._socket;
        _result = other._result;
        _pendingResponses = other._pendingResponses;
    }
    return *this;
}

Epoll::~Epoll()
{

}

void Epoll::closeFd()
{
    for (unsigned int i = 0; _socket.size() != i; ++i)
        close(_socket[i]);
    close(_epollfd);
}

void Epoll::bindSocket(const std::string &host, const unsigned int &port)
{
    _socket.push_back(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0));
    int socket = _socket.back();
    if (socket == -1)
        throw std::runtime_error("server socket create failed");
    sockaddr_in addr;
    addrinfo hints;
    addrinfo* result;
    int opt = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("setsockopt SO_REUSEADDR failed");
    memset(&addr, 0, sizeof(addr));
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int status = getaddrinfo(host.c_str(), NULL, &hints, &result);
    if (status != 0)
        throw std::runtime_error("getaddrinfo failed");
    addr.sin_addr = ((sockaddr_in *)(result->ai_addr))->sin_addr;
    freeaddrinfo(result);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (bind(socket, (sockaddr*)&addr, sizeof(addr)) == -1)
        throw std::runtime_error("socket bind failed");
    if (listen(socket, SOMAXCONN) == -1)
        throw std::runtime_error("socket listen failed");
    std::cout << "socket {host : " << host << ", port : " << port << "} is opened" << std::endl;
}

void Epoll::initSocket()
{
    if (_epollfd == -1)
        throw std::runtime_error("epoll create failed");
    typedef std::pair<std::string, unsigned int> hostPortPair;
    std::vector<hostPortPair> checkport;
    
    for (unsigned int i = 0; i < _config.size(); ++i)
    {
        for (unsigned int j = 0; j < _config[i].size(); ++j)
        {
            std::string host = _config[i][j].getHost();
            std::vector<unsigned int> ports = _config[i][j].getPort();
            
            for (unsigned int k = 0; k < ports.size(); ++k)
            {
                hostPortPair newPair = std::make_pair(host, ports[k]);
                bool isDuplicate = false;
                
                for (unsigned int m = 0; m < checkport.size(); ++m)
                {
                    if (checkport[m].first == host && checkport[m].second == ports[k])
                    {
                        isDuplicate = true;
                        break;
                    }
                }
                if (!isDuplicate)
                    checkport.push_back(newPair);
            }
        }
    }
    for (unsigned int i = 0; i < checkport.size(); ++i)
        bindSocket(checkport[i].first, checkport[i].second);
}

int Epoll::isServerSocket(int &fd)
{
    std::vector<int>::iterator i = std::find(_socket.begin(), _socket.end(), fd); 
    if (i == _socket.end())
        return 0;
    return *i;
}

void Epoll::handleNewConnection(int &fd)
{
    // while (1)
    // {
        sockaddr addr;
        socklen_t sockSize = sizeof(addr);
        int clientSock = accept(fd, (struct sockaddr*)&addr, &sockSize);
        if (clientSock == -1)
        {
        //    if (errno == EAGAIN || errno == EWOULDBLOCK) 
        //         break;
            throw std::runtime_error("client accept failed");
        }

        epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = clientSock;
        if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, clientSock, &ev ) == -1)
        {
            close(clientSock);
            throw std::runtime_error("client register failed");
        } 
    // }
}

void Epoll::handleRead(int &fd)
{
    char buffer[5];
    int bytesRead = recv(fd, buffer, sizeof(buffer), 0);

    if (bytesRead > 0) {
        _result[fd].append(buffer, bytesRead);
        // if (result.find("favicon") == std::string::npos)
        if (_result[fd].size() >= 4 && _result[fd].substr(_result[fd].size() - 4) == "\r\n\r\n")
        {
            std::cout << "Received complete request:\n" << _result[fd] << std::endl;

            Request request(&_config);
            request.parse(_result[fd]);
            request.debug();

            GetHandler getHandler;
            getHandler.handleRequest(request);
            epoll_event ev;
            ev.events = EPOLLOUT;
            ev.data.fd = fd;
            if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd, &ev) == -1)
            {
                handleClose(fd);
                return;
            }
        }
    }
    // else if (bytesRead == 0) {
    //     //evs.erase(fd);
    //     epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL);
    //     close(fd);
    // }
}

std::string createHttpResponse(const std::string& filePath) {
    // HTML 파일 읽기
    std::ifstream file(filePath.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    // 파일 내용 읽기
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    // HTTP 응답 생성   
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: " << content.length() << "\r\n"
             << "\r\n"
             << content;

    return response.str();
}

std::string executeCgi(const char *(&args)[3])
{
    int fd[2];
    pid_t pid;
    
    if (pipe(fd) == -1)
        throw std::runtime_error("Failed to pipe");
    pid = fork();
    if (pid == -1)
    {
        close (fd[0]);
        close (fd[1]);
        throw std::runtime_error("Failed to fork");
    }
    if (pid == 0)
    {
        close (fd[0]);
        if (dup2(fd[1], 1) == -1)
        {
            close (fd[1]);
            throw std::runtime_error("Failed to fork");
        }
        close (fd[1]);
        if (execve(args[0], (char *const *)args, NULL) == -1)
            throw std::runtime_error("Failed to exec");
    }
    close(fd[1]);
    char buffer[4096];
    std::string res;
    ssize_t bytes;
    while ((bytes = read(fd[0], buffer, sizeof(buffer))) > 0)
        res.append(buffer, bytes);
    close(fd[0]);
    std::cout << res << std::endl;
    int status;
    waitpid(pid, &status, 0);
    std::stringstream full_response;
    full_response << "HTTP/1.1 200 OK\r\n";
    if (!strcmp("/usr/bin/python3", args[0]))
        full_response << "Content-Type: text/html\r\n";
    else
        full_response << "Content-Type: image/jpeg\r\n";
    full_response << "Content-Length: " << res.length() << "\r\n\r\n";
    full_response << res;
    std::cout << full_response.str() << std::endl;
    return full_response.str();
}

void Epoll::handleWrite(int &fd)
{
    if (_pendingResponses.find(fd) == _pendingResponses.end())
    {
        const char *py[] = {
            "/usr/bin/python3",
            "python.py",
            NULL
        };
        _pendingResponses[fd] = executeCgi(py);

        // const char *cpp[] = {
        //     "a.out",
        //     NULL,
        //     NULL
        // };
        // _pendingResponses[fd] = executeCgi(cpp);
        // if (_result[fd].find("GET /styles.css") != std::string::npos) {
        //     _pendingResponses[fd] = create_css_response();
        // }
        // else if (_result[fd].find("GET /favi/background") != std::string::npos) {
        //     _pendingResponses[fd] = create_image_response();
        // } else if (_result[fd].find("GET /favicon.ico") != std::string::npos) {
        //     _pendingResponses[fd] = create_favi_response();
        // } else if (_result[fd].find("GET /favi/gyeongju") != std::string::npos) {
        //     _pendingResponses[fd] = create_test_image_response();
        // // }
        // } else if (_result[fd].find("Cookie") == std::string::npos) {
        //     _pendingResponses[fd] = create_cookie_http_response();
        // }
        // else {
        //     _pendingResponses[fd] = create_http_response();
        // }
    }
    // std::cout << _pendingResponses[fd].size() << std::endl;
    size_t to_send = std::min(_pendingResponses[fd].size(), static_cast<size_t>(5));
    int bytes_sent = send(fd, _pendingResponses[fd].c_str(), to_send, 0);
    if (bytes_sent > 0)
    {
        _pendingResponses[fd] = _pendingResponses[fd].substr(bytes_sent);

        if (_pendingResponses[fd].empty())
        {
            _pendingResponses.erase(fd);
            _result.clear();
            epoll_event ev;
            ev.events = EPOLLIN;
            ev.data.fd = fd;
            epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd, &ev);
        }
    }
}

void Epoll::handleClose(int &fd)
{
    epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL);
    if (_result.find(fd) != _result.end())
        _result.erase(fd);
    close(fd);
    std::cout << "Client disconnected: fd " << fd << std::endl;
    // throw std::runtime_error("the client has been disconnected.");
}

void Epoll::registerSeverSocket()
{
    epoll_event ev;
    ev.events = EPOLLIN;

    for (unsigned int i = 0; i != _socket.size(); ++i)
    {
        ev.data.fd = _socket[i];
        if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _socket[i], &ev) == -1)
            throw std::runtime_error("epoll wait failed");
    }
}

void Epoll::initClient()
{
    epoll_event events[MAX_EVENTS];

    while (true)
    {
        int nfds = epoll_wait(_epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1)
            throw std::runtime_error("epoll wait failed");
        for (int i = 0; i < nfds; ++i)
        {
            int serverSocket = isServerSocket(events[i].data.fd);

            if (serverSocket)
                handleNewConnection(serverSocket);
            else
            {
                if (events[i].events & EPOLLIN)
                    handleRead(events[i].data.fd);
                if (events[i].events & EPOLLOUT)
                    handleWrite(events[i].data.fd);
                if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
                    handleClose(events[i].data.fd);
            }
        }
    }
}

void Epoll::run()
{
    _epollfd = epoll_create(42);
    initSocket();
    registerSeverSocket();
    initClient();
}
