#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "../Inc/WebServer.hpp"
#include "../RequestHeader/Request.hpp"


class Response {
    private:
        std::string _header;
        std::string _body;
        unsigned long long _contentLength;
        std::string getErrorPath(int error);
        std::string getErrorHeader(int error);

    public:
        // 생성자
        Response(): _header(""), _body(""), _contentLength(0) {}
        ~Response() {};

        //RequestHandler
        std::string RequestHandler(Request& request);
        std::string textHandler(const Request& request, const std::string& accept);
        std::string imageHandler(const Request& request, const std::string& accept);
        std::string errorHandler(int error);
        std::string postHandler(Request& request);
        std::string autoIndexHeader(const std::string& body);
        std::string autoIndexHandler(const std::string& mapPath, const std::string& path);
        std::string removeHandler(Request& request);
        std::string cgiHandler(Request& request);
        std::string executeCgi(const std::vector<std::string>& cgiArgv);


        int validateRequest(Request& request);
        int getValidate(Request& request);
};

#endif