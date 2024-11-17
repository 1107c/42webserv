#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "../Inc/WebServer.hpp"
#include "../RequestHeader/Request.hpp"


class Response {
    private:
        std::string _header;
        std::string _body;
        unsigned long long _contentLength;
            
    public:
        // 생성자
        Response();
        ~Response();

        //메시지 생성 함수
        void makeResponseRedirectionMessage(Request& request);
        void makeResponseGetMessage(Request& request);
        void makeErrorMessage(int status);

        //리다이렉션할 파일 정보
        bool getRedirectionFile(Request& request);
        bool getErrorFile(int statusCode);
        bool getFaviconFile(Request& request);

        //게터
        std::string getResponseMessage() const;
};

#endif