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
        int locationIdx = 0;
        //로케이션 블록이 있을 경우
        if (_path == "/") {
            for(size_t i = 1; i < (*_conf)[serverBlockIdx].size(); i++) {
                //_path와 .getPath()의 값이 얼마만큼 같냐
                if (_path == (*_conf)[serverBlockIdx][i].getPath()) {
                    // if (!(*_conf)[serverBlockIdx][i].getRedirect().empty())
                    locationIdx = i;
                    break;
                }
            }
        } else {
            locationIdx = getLocationBlock(serverBlockIdx);
        }
        _location = (*_conf)[serverBlockIdx][locationIdx];
    }
    return true;
}

int Request::getLocationBlock(int& serverBlockIdx) {
    int max = -1;
    ssize_t index = -1;
    int tmp;
    std::string dir = _path;
    if (dir[dir.size() - 1] != '/')
    {
        for (int i = dir.size() - 1; i > -1; --i)
        {
            if (dir[i] == '/')
            {
                dir = dir.substr(0, i + 1);
                break;
            }
        }
    }
    // std::cout <<"@@@DIR:"<<dir <<std::endl;
    // "/"단위로 끊겨야돼
    for(size_t locationIdx = 1; locationIdx < (*_conf)[serverBlockIdx].size(); locationIdx++) {
        //_path와 .getPath()의 값이 얼마만큼 같냐
        tmp = ft_strncmp(_path, (*_conf)[serverBlockIdx][locationIdx].getPath());

        // if ((*_conf)[serverBlockIdx][locationIdx].getPath() == '/')
        // std::cout << "this :" << _path << std::endl;
        // std::cout << "this :" << (*_conf)[serverBlockIdx][locationIdx].getPath() << std::endl;

        if (max < tmp && (*_conf)[serverBlockIdx][locationIdx].getPath() != _path 
        && !(*_conf)[serverBlockIdx][locationIdx].getRedirect().empty()) {
            max = tmp;
            index = locationIdx;
        } //리다이렉션일 경우 유사도
        else if ((*_conf)[serverBlockIdx][locationIdx].getRedirect().empty() && 
        (*_conf)[serverBlockIdx][locationIdx].getPath() == _path)
        {
            return locationIdx;
        } // 완전히 일치할 경우
    }
    if (index == -1)
    {
        for(size_t locationIdx = 1; locationIdx < (*_conf)[serverBlockIdx].size(); locationIdx++)
        {
            if (dir == (*_conf)[serverBlockIdx][locationIdx].getPath())
                return locationIdx;
        }
        return 0;
    }      
    return index;
}