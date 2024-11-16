#include "../RequestHeader/Request.hpp"

//내부 헬퍼 함수


void Request::setError(int code) { // 에러 설정
    this->_errorCode = code;
}

void Request::normalizedPath()
{
    //  std::cout << (*_conf).
    std::cout <<"this> "<< _path << std::endl;
     // 경로 정규화 (../와 ./ 처리)
//구현 필요
}

bool Request::validateRequest() { // 요청의 유효성 검사
    //1. Request메시지 확인 후(서버 네임, 포트 번호) 알맞은 서버 블록 가져오기
    Location location;
    int findIdx = -1;
    unsigned int serverPort;
    std::stringstream ss(this->getServerPort());
    ss >> serverPort;

    if (ss.fail()) {
        setError(400);
        return false;
    }
    for (size_t totalIdx = 0; totalIdx < this->_conf->size(); totalIdx++) {
        std::cout << ((*_conf)[totalIdx][0].getServerName())[0] <<std::endl;

        for(size_t nameIdx = 0; nameIdx < ((*_conf)[totalIdx][0].getServerName()).size(); nameIdx++) {
            if (this->getServerName() == ((*_conf)[totalIdx][0].getServerName())[nameIdx]) {
                for (size_t portIdx = 0; portIdx < (*_conf)[totalIdx][0].getPort().size(); portIdx++) {
                    if (serverPort == ((*_conf)[totalIdx][0].getPort()[portIdx])) {
                        findIdx = totalIdx;
                        this->_serverBlockIdx = findIdx;
                        break ;
                    }
                }
                if (findIdx != -1)
                    break ;
            }
        }
        if (findIdx != -1)
            break ;
    }

    if (findIdx == -1) {
        setError(400);
        return false;
    }
    //2. 서버 블록에 맞게 허용 메소드 확인하기
    //std::cout << location.getMethod() << std::endl;
    int methodFindIdx = -1;
    for(size_t methodIdx = 0; methodIdx < (*_conf)[findIdx][0].getMethods().size(); methodIdx++) {
        if (this->getMethod() == (*_conf)[findIdx][0].getMethods()[methodIdx]) {
            methodFindIdx = methodIdx;
        }
    }
    if (methodFindIdx == -1) {
        setError(400);
        return false;
    }

    //3. body 부가 있다면, max body size 확인하기
    if (this->_contentLength > (*_conf)[findIdx][0].getClientMaxBodySize()) {
        setError(400);
        return false;
    }

    return true;
}

void Request::clear() { //요청 객체 초기화
//구현 필요
}

