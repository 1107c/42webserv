#include "../RequestHeader/Request.hpp"

//내부 헬퍼 함수


void Request::setError(int code) { // 에러 설정
    this->_errorCode = code;
}

void Request::normalizedPath() { // 경로 정규화 (../와 ./ 처리)
//구현 필요
}

bool Request::validateRequest() const { // 요청의 유효성 검사
//구현 필요
    return true;
}

void Request::clear() { //요청 객체 초기화
//구현 필요
}

