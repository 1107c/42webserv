#include "../RequestHeader/Request.hpp"

bool Request::isCorretPort(size_t& serverblockIdx) {
    std::vector<unsigned int> port = (*_conf)[serverblockIdx][0].getPort();

    for(size_t portIdx = 0; portIdx < port.size(); portIdx++) {
        if (ToString(port.at(portIdx)) == _port)
            return true;
    }
    return false;
}

bool Request::getConfigOption() {
    int serverBlockIdx = -1;
    for(size_t idx = 0; idx < this->_conf->size(); idx++) {
        if (_serverName == (*_conf)[idx][0].getHost() && isCorretPort(idx)) {
            serverBlockIdx = idx;
            break ;
        }
    }
    if (serverBlockIdx == -1) {
        return false;
    }


    int locationIdx = -1;
    //로케이션 블록이 없을 경우
    if ((*_conf)[serverBlockIdx].size() == 1) {
        locationIdx = 0;
        _location = (*_conf)[serverBlockIdx][0];
    } else {
        int locationIdx = 0;
        //로케이션 블록이 있을 경우
        if (_path == "/") {
            for(size_t i = 1; i < (*_conf)[serverBlockIdx].size(); i++) {
                //_path와 .getPath()의 값이 얼마만큼 같냐
                if (_path == (*_conf)[serverBlockIdx][i].getPath()) {
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
    size_t index = -1;
    int tmp;

    for(size_t locationIdx = 1; locationIdx < (*_conf)[serverBlockIdx].size(); locationIdx++) {
        //_path와 .getPath()의 값이 얼마만큼 같냐
        tmp = ft_strncmp(_path, (*_conf)[serverBlockIdx][locationIdx].getPath());
        if (max < tmp) {
            max = tmp;
            index = locationIdx;
        }
    }

    return index;
}