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
        if (headerEnd == std::string::npos) {
            setError(400);
            return false;
        }

        std::string startLine = rawRequest.substr(0, firstLine);
        if (!parseStartLine(startLine)) {
            setError(400);
            return false;
        }



        //3. 헤더 파싱
        std::string headers = rawRequest.substr(firstLine + 2, headerEnd - firstLine - 2);
        if (!parseHeaders(headers)) {
            setError(400);
            return false;
        }
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

	if (this->_headers.count("Cookie")) {
		try {
            std::string cookie;
            std::istringstream iss(this->_headers.at("Cookie"));
            if (!(iss >> cookie)) {
                setError(400);
                return (false);
            }
            //문자열 쪼개기
            size_t colonPos = cookie.find("=");
            if (colonPos == std::string::npos) {
                setError(400);
                return false;
            }
            this->_cookie = cookie.substr(colonPos + 1);
			// std::cout << "Cookie: " << _cookie << "\n";
        } catch (...) {
            setError(400);
            return false;
        }
	}

    return true;
}

bool Request::parseBody(const std::string& bodyContent) {
    if (isChunked()) {
        //chunked encoding 처리
        return parseChunkedBody(bodyContent);
    } else if (this->_contentLength > 0) {
        //일반 바디 처리
        std::cout << bodyContent << std::endl << "BodyLength: " << bodyContent.length() << " contentLength: " << this->_contentLength << std::endl;
        if (bodyContent.length() != this->_contentLength) {
            std::cout << "가끔씩 post를 여러번 하다보면 requestparse.cpp 여기서 오류가 발생함 원인 불명" <<std::endl;
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
        this->_query = url.substr(queryPos + 1);
    } else {
        this->_path = url.substr(0);
    }
}