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
    return true;
}

int Request::getLocationBlock(int& serverBlockIdx) {

    std::string file,str = _path;
    size_t pos ;

    while (1)
    {
        if (str[str.size() - 1] == '/')
        {
            str.erase(str.size() - 1);
            for(size_t locationIdx = 1; locationIdx < (*_conf)[serverBlockIdx].size(); locationIdx++) {   
                if (str == (*_conf)[serverBlockIdx][locationIdx].getPath())
                    return locationIdx;
            }

        }
        pos = str.rfind('/');
        if(pos == std::string::npos)
            break;
        file = str.substr(pos + 1);
        str = str.substr(0,pos + 1);
    }
    return 0;
}