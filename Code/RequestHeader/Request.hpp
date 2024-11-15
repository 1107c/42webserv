#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../Inc/WebServer.hpp"
#include "../ConfigureHeader/Conf.hpp"
#include "../ConfigureHeader/Location.hpp"


class Request {
    private:
        // HTTP 요청의 주요 구성요소
        std::string _method;        // GET, POST, DELETE 등
        std::string _path;      // 요청 경로 (/index.html 등)
        std::string _query;   // 쿼리 스트링
        std::string _version;      // HTTP 버전 (HTTP/1.1 등)
        std::string _serverName; //서버 네임
        std::string _port;  //서버 포트
        const std::vector<std::vector<Location> > *_conf; //conf파일 서버 블락
        int _serverBlockIdx;
        std::string _mappingUrl;

        // 헤더와 바디
        std::map<std::string, std::string> _headers;  // 요청 헤더들
        std::string _body;      // 요청 바디
            
        // 파싱 관련 상태
        bool _isParsed;
        bool _isComplete;
        size_t _contentLength;
            
        // 에러 관리
        int _errorCode;
            
    public:
        // 생성자
        Request();
        Request(const std::vector<std::vector<Location> > *conf);
        ~Request();

        // 파싱 메소드
        bool parse(const std::string& rawRequest);
        bool parseStartLine(const std::string& line);
        bool parseHeaders(const std::string& headerSection);
        bool parseBody(const std::string& bodyContent);
        bool parseChunkedBody(const std::string& chunkedBody);
            
        // Getter 메소드들
        const std::string& getMethod() const;
        const std::string& getPath() const;
        const std::string& getQuery() const;
        const std::string& getVersion() const;
        const std::string& getBody() const;
        const std::string& getServerName() const;
        const std::string& getServerPort() const;
        const std::string& getMappingUrl() const;
        const std::map<std::string, std::string>& getHeaders() const;
        std::string getHeader(const std::string& key) const;
        int getServerBlockIdx() const;
        const std::vector<std::vector<Location> > *getConfig() const;

        // 상태 확인 메소드들
        bool isChunked() const;  // chunked transfer-encoding 확인
        bool isComplete() const;
        bool hasError() const;
        int getErrorCode() const;
        size_t getContentLength() const;  // Content-Length 값 반환
        bool isMethodAllowed(const std::vector<std::string>& allowedMethods) const;


        // Utility 메소드들
        void clear();  // 요청 객체 초기화
        void debug(); //안에 내용 출력
        
    private:
        // 내부 헬퍼 메소드들
        void parseQueryString(const std::string& url);
        void setError(int code);  // 에러 설정
        void normalizedPath();  // 경로 정규화 (../와 ./ 처리)
        bool findDot();
        void redirectionPath();
        bool validateRequest();  // 요청의 유효성 검사

};

#endif