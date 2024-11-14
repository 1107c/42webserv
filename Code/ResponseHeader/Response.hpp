#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "../Inc/WebServer.hpp"
#include "../RequestHeader/Request.hpp"


class Response {
    private:
        std::string _header;
        std::string _body;
            
    public:
        // 생성자
        Response();
        ~Response();

        //메인 함수
        void makeResponseRedirectionMessage(Request& request);

        //리다이렉션할 파일 정보
        unsigned long long getRedirectionFileLength(Request& request);
        std::string getRedirectionFileContent(Request& request);

        //게터
        std::string getResponseMessage() const;
};

#endif