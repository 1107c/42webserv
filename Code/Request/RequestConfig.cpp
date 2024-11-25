#include "../RequestHeader/Request.hpp"

bool Request::isCorretPort(size_t& serverblockIdx) {
    std::vector<unsigned int> port = (*_conf)[serverblockIdx][0].getPort();

    for(size_t portIdx = 0; portIdx < port.size(); portIdx++) {
        if (ToString(port.at(portIdx)) == _port)
            return true;
    }
    return false;
}

bool  Request::isCorreectServername(size_t &idx)
{
    std::vector<std::string> server = (*_conf)[idx][0].getServerName();

    for(size_t serverIdx = 0; serverIdx < server.size(); serverIdx++) {
        if (ToString(server.at(serverIdx)) == _serverName)
            return true;
    }
    return false;
}

bool Request::getConfigOption() {
    int serverBlockIdx = -1;
    for(size_t idx = 0; idx < this->_conf->size(); idx++) {
        if (isCorreectServername(idx) && isCorretPort(idx)) {
            serverBlockIdx = idx;
            break ;
        }
    }
    if (serverBlockIdx == -1) {

        setError(400);
        return false;
    }

    //로케이션 블록이 없을 경우
    if ((*_conf)[serverBlockIdx].size() == 1) {
        _location = (*_conf)[serverBlockIdx][0];
    } else {
        _location = (*_conf)[serverBlockIdx][getLocationBlock(serverBlockIdx)];
    }
    std::cout << "@@@@@"<< _location.getPath()<<std::endl;
    return true;
}

int Request::getLocationBlock(int& serverBlockIdx) {
    size_t size = 0;
    size_t pos = _path.rfind('/');
    std::string temp = _path;
    while (pos != std::string::npos) {
        size++;
        temp = temp.substr(0, pos);
        pos = temp.find('/');
    }
    std::string str = _path;
    for (size_t i = 0; i <= size; ++i) {
        for (size_t j = 0; j < (*_conf)[serverBlockIdx].size(); ++j)
        {
            // std::cout << "getpath :" << (*_conf)[serverBlockIdx][j].getPath() <<std::endl; 
            if ((*_conf)[serverBlockIdx][j].getPath() == str)
                return j;
        }
        // std::cout <<"b:str:"<<str << std::endl;
        pos = str.rfind('/');
        if (pos == 0)
            str = "/";
        else if (i != size)
            str = str.substr(0, pos);
        else
            str = "/";
        // std::cout <<"a:str:"<<str << std::endl;

    }
    return 0;
}