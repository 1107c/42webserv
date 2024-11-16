#ifndef GETHANDLER_HPP
# define GETHANDLER_HPP

#include "../Inc/WebServer.hpp"
#include "../RequestHeader/Request.hpp"
#include "../ResponseHeader/Response.hpp"

class GetHandler {
    private:
        // 리다이렉션 관련 상수
        static const int MAX_REDIRECTS = 5;  // 최대 리다이렉션 횟수
       bool _isRedirection;
        struct RedirectConfig {
            std::string from_path;
            std::string to_path;
            int status_code;  // 301(영구) 또는 302(임시)
        };
        

    public:
        GetHandler();
        
        // GET 요청 처리 메인 함수
        bool handleRequest(Request &requestMessage);

        //리다이렉션 요청 확인
        bool checkRedirection(Request& request);
        bool checkRedirectionCondition(Request& request);
        bool checkRedirectionConfig(Request& request);


        //게터
        bool getIsRedirection() const;
};

#endif