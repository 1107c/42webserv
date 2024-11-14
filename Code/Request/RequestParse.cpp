#include "../RequestHeader/Request.hpp"

bool Request::parse(const std::string& rawRequest) {
    try {
        //1. 요청을 줄 단위로 분리
        size_t headerEnd = rawRequest.find("\r\n\r\n");
        if (headerEnd == std::string::npos) {
            setError(400);
            return false;
        }

        //2. start line 파싱
        ssize_t firstLine = rawRequest.find("\r\n");
        std::string startLine = rawRequest.substr(0, firstLine);
        if (!parseStartLine(startLine)) {
            return false;
        }

        //3. 헤더 파싱
        std::string headers = rawRequest.substr(firstLine + 2, headerEnd - firstLine - 2);
        if (!parseHeaders(headers)) {
            return false;
        }

        //4. 바디 파싱 (있는 경우)
        if (headerEnd + 4 < rawRequest.length()) {
            std::string body = rawRequest.substr(headerEnd + 4);
            if (!parseBody(body)) {
                return false;
            }
        }
        this->_isParsed = true;

        //안에서 iscomplete flag 세워야함
        validateRequest();
    } catch (const std::exception& e) {
        setError(400);
        return false;
    }
    return true;
}

bool Request::parseStartLine(const std::string& line) {
    std::istringstream iss(line);
    std::string method, url, version;

    if (!(iss >> method >> url >> version)) {
        setError(400);
        return (false);
    }

    this->_method = method;
    parseQueryString(url); // URL에서 path, query 분리
    this->_version = version;

    return true;
}

bool Request::parseHeaders(const std::string& headerSection) {
    std::istringstream iss(headerSection);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.empty() || line == "\r")
            continue;

        size_t colonPos = line.find(":");
        if (colonPos == std::string::npos) {
            setError(400);
            return false;
        }
        
        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);

        //앞뒤 공백 제거
        value.erase(0, value.find_first_not_of(" "));
        value.erase(value.find_last_not_of(" \r") + 1);

        this->_headers[key] = value;
    }

    //HTTP Body부의 Content-Length 처리
    if (this->_headers.count("Content-Length")) {
        try {
            std::istringstream(this->_headers.at("Content-Length")) >> this->_contentLength;
        } catch (...) {
            setError(400);
            return false;
        }
    }

    //Server name, port 멤버 변수로 넣기
    if (this->_headers.count("Host")) {
        try {
            std::string hostString;
            std::istringstream iss(this->_headers.at("Host"));
            if (!(iss >> hostString)) {
                setError(400);
                return (false);
            }
            //문자열 쪼개기
            size_t colonPos = hostString.find(":");
            if (colonPos == std::string::npos) {
                setError(400);
                return false;
            }
            this->_serverName = hostString.substr(0, colonPos);
            this->_port = hostString.substr(colonPos + 1);
        } catch (...) {
            setError(400);
            return false;
        }
    } else {
        setError(400);
        return false;
    }

    return true;
}

//chunked되었다고 명시되어있지 않으면 바디부 그냥 무시됌 따로 에러처리 없음
bool Request::parseBody(const std::string& bodyContent) {
    if (isChunked()) {
        //chunked encoding 처리
        return parseChunkedBody(bodyContent);
    } else if (this->_contentLength > 0) {
        //일반 바디 처리
        if (bodyContent.length() != this->_contentLength) {
            setError(400);
            return false;
        }
        this->_body = bodyContent;
    }
    return true;
}

//구현해야함
bool Request::parseChunkedBody(const std::string& chunkedBody) {
    (void)chunkedBody;
    return (true);
}

void Request::parseQueryString(const std::string& url) {
    size_t queryPos = url.find('?');

    //Query string 처리
    if (queryPos != std::string::npos) {
        this->_path = url.substr(0, queryPos);
        //뒤에 공백있는지 확인 해야 할 것 같기도..
        this->_query = url.substr(queryPos + 1);
        std::cout << "Query parsing : " << this->_query << std::endl;
    } else {
        this->_path = url.substr(0);
    }

    normalizedPath();
}