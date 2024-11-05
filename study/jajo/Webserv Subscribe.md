>This is when you finally understand why a URL starts with HTTP

# Summary
- This project is about writing your own HTTP server.
- 이 프로젝트는 나만의 HTTP 서버를 작성하는 것입니다.

- You will be able to test it with an actual browser.
- 실제 브라우저로 테스트할 수 있습니다.

- HTTP is one of the most used protocols on the internet.
-  HTTP는 인터넷에서 가장 많이 사용되는 프로토콜 중 하나입니다.

- Knowing it arcane will be useful, even if you won't be working on a website.
- 웹 사이트에서 작업하지 않더라도 이 프로토콜을 아는 것은 유용할 것입니다.

# Introduction
The Hypertext Transfer Protocol (HTTP) is an application protocol for distributed, collaborative, hypermedia information systems. 
하이퍼텍스트 전송 프로토콜(HTTP)은 분산형 협업 하이퍼미디어 정보 시스템을 위한 애플리케이션 프로토콜입니다. 

HTTP is the foundation of data communication for the World Wide Web, where hypertext documents include hyperlinks to other resources that the user can easily access. 
HTTP는 사용자가 쉽게 액세스할 수 있는 다른 리소스에 대한 하이퍼링크를 포함하는 월드와이드웹의 데이터 통신 기반입니다. 

For example, by a mouse click or by tapping the screen in a web browser. 
예를 들어, 마우스 클릭이나 웹 브라우저에서 화면을 탭하는 등의 방법으로 다른 리소스에 액세스할 수 있습니다. 

HTTP was developed to facilitate hypertext and the World Wide Web. 
HTTP는 하이퍼텍스트와 월드 와이드 웹을 용이하게 하기 위해 개발되었습니다

The primary function of a web server is to store, process, and deliver web pages to clients. 
웹 서버의 주요 기능은 웹 페이지를 저장, 처리 및 클라이언트에 전달하는 것입니다. 

The communication between client and server takes place using the Hypertext Transfer Protocol (HTTP). 
클라이언트와 서버 간의 통신은 하이퍼텍스트 전송 프로토콜(HTTP)을 사용하여 이루어집니다. 

Pages delivered are most frequently HTML documents, which may include images, style sheets, and scripts in addition to the text content. 
전송되는 페이지는 대부분 HTML 문서이며, 텍스트 콘텐츠 외에 이미지, 스타일시트, 스크립트 등이 포함될 수 있습니다.

Multiple web servers may be used for a high-traffic website. 
트래픽이 많은 웹사이트에는 여러 웹 서버를 사용할 수 있습니다. 

A user agent, commonly a web browser or web crawler, initiates communication by requesting a specific resource using HTTP and the server responds with the content of that resource or an error message if unable to do so. 
일반적으로 웹 브라우저 또는 웹 크롤러인 사용자 에이전트는 HTTP를 사용하여 특정 리소스를 요청하여 통신을 시작하고 서버는 해당 리소스의 콘텐츠 또는 응답할 수 없는 경우 오류 메시지로 응답합니다. 

The resource is typically a real file on the server’s secondary storage, but this is not necessarily the case and depends on how the webserver is implemented. 
리소스는 일반적으로 서버의 보조 저장소에 있는 실제 파일이지만 반드시 그런 것은 아니며 웹 서버가 구현된 방식에 따라 달라집니다.

While the primary function is to serve content, full implementation of HTTP also includes ways of receiving content from clients. 
주요 기능은 콘텐츠를 제공하는 것이지만 HTTP의 완전한 구현에는 클라이언트로부터 콘텐츠를 수신하는 방법도 포함됩니다.

This feature is used for submitting web forms, including the uploading of files.
이 기능은 파일 업로드를 포함한 웹 양식 제출에 사용됩니다.

# General rules
• Your program should not crash in any circumstances (even when it runs out of memory), and should not quit unexpectedly. If it happens, your project will be considered non-functional and your grade will be 0. 
프로그램이 어떤 상황에서도(메모리가 부족한 경우에도) 충돌해서는 안 되며 예기치 않게 종료되어서는 안 됩니다.
충돌이 발생하면 프로젝트가 작동하지 않는 것으로 간주되어 성적이 0점이 됩니다.

• You have to turn in a Makefile which will compile your source files. It must not relink. 
소스 파일을 컴파일하는 메이크파일을 제출해야 합니다. 링크가 다시 연결되지 않아야 합니다.

• Your Makefile must at least contain the rules: $(NAME), all, clean, fclean and re. • Compile your code with c++ and the flags -Wall -Wextra -Werror 
메이크파일에는 최소한 다음 규칙이 포함되어야 합니다:
(NAME), all, clean, fclean, re.
c++와 -Wall -Wextra -Werror 플래그를 사용하여 코드를 컴파일합니다.

• Your code must comply with the C++ 98 standard. Then, it should still compile if you add the flag -std=c++98. 
코드는 C++ 98 표준을 준수해야 합니다. 그런 다음 -std=c++98 플래그를 추가해도 컴파일됩니다.

• Try to always develop using the most C++ features you can (for example, choose over ). You are allowed to use C functions, but always prefer their C++ versions if possible. 
항상 가능한 한 많은 C++ 기능을 사용하여 개발하세요(예: <string.h> 대신 cstring 선택). 
C 함수를 사용할 수 있지만 가능하면 항상 C++ 버전을 선호하세요.

• Any external library and Boost libraries are forbidden.
외부 라이브러리 및 부스트 라이브러리는 금지됩니다.

# Mandatory part

### Program name
- webserv

### Turn in files
- Makefile
- `*.h, *cpp, *.tpp, *.ipp`
- configuration files

### Makefile
- Name, all clean, fclean, re

### External functs.

##### Everything in C++98.
##### Signal
- [[kill]]
- [[signal]]

##### 파일 입출력
- [[open]]
- [[read]]
- [[write]]
- [[close]]
- [[dup, dup2]]
##### 파일 속성과 디렉토리
- [[opendir]]
- [[readdir]]
- [[closedir]]
- [[stat]]
- [[access]]

##### 프로세스
- [[execve]]
- [[waitpid]]
- [[fork]]

##### 프로세스 간 통신
- [[pipe]]

##### 에러
- [[strerror]]
- [[gai_strerror]] ->설명 추가 필요

##### 고급 입출력
- fcntl

##### 소켓 [[Socket 통신]]
- [[socket]]
- [[bind]]
- [[listen]]
- [[accept]]
- [[connect]]
- [[read]]
- [[write]]
- [[close]]
- [[send]]
- [[recv]]

- [[htons]] (host-to-network short)
- noths (network-to-host short)
- htonl (host-to-network long)
- nothl (network-to-host long)

- socketpair
- setsockopt
- getsockname





- errno


- select
- poll
- epoll
- epoll_create
- epoll_ctl
- epoll_wait
- kqueue
- kqueue
- kevent

- chdir bind
- 
- getaddrinfo
- freeaddrinfo
- 
- getprotobyname






### Description
A HTTP server in C++ 98

Your excutable will be run as fllows:
./webserv `[configuration file]`

Even if poll() is mentionned in the subject and the evaluation scale, you can use any equivalent such as select(), kqueue(), or epoll().
제목과 평가 척도에 poll()이 언급되어 있더라도 select(), kqueue() 또는 epoll()과 같은 동등한 함수를 사용할 수 있습니다.

Please read the RFC and do some tests with telnet and NGINX before starting this project.
이 프로젝트를 시작하기 전에 RFC를 읽고 텔넷과 NGINX로 몇 가지 테스트를 해보세요.

Even if you don’t have to implement all the RFC, reading it will help you develop the required features.
모든 RFC를 구현할 필요는 없더라도 읽어보면 필요한 기능을 개발하는 데 도움이 될 것입니다.

# Requirments
• Your program has to take a configuration file as argument, or use a default path. 
- 프로그램은 구성 파일을 인수로 받거나 기본 경로를 사용해야 합니다.

• You can’t execve another web server. 
- 다른 웹 서버를 실행할 수 없습니다. 

• Your server must never block and the client can be bounced properly if necessary. 
- 서버는 절대로 차단해서는 안 되며 필요한 경우 클라이언트가 제대로 바운스될 수 있어야 합니다. 

• It must be non-blocking and use only 1 poll() (or equivalent) for all the I/O operations between the client and the server (listen included). 
- 클라이언트와 서버 간의 모든 I/O 작업(수신 포함)에 대해 비차단성이어야 하며 poll()(또는 이에 상응하는 함수)을 1개만 사용해야 합니다. 

• poll() (or equivalent) must check read and write at the same time. 
- poll()(또는 이와 동등한 함수)는 읽기와 쓰기를 동시에 확인해야 합니다. 

• You must never do a read or a write operation without going through poll() (or equivalent). 
- poll()(또는 이와 동등한 함수)를 거치지 않고 읽기 또는 쓰기 작업을 수행해서는 안 됩니다.

• Checking the value of errno is strictly forbidden after a read or a write operation. 
 - 읽기 또는 쓰기 작업 후 errno 값을 확인하는 것은 엄격히 금지되어 있습니다. 

• You don’t need to use poll() (or equivalent) before reading your configuration file. 

Because you have to use non-blocking file descriptors, it is possible to use read/recv or write/send functions with no poll() (or equivalent), and your server wouldn’t be blocking. But it would consume more system resources. 

Thus, if you try to read/recv or write/send in any file descriptor without using poll() (or equivalent), your grade will be 0.
- 구성 파일을 읽기 전에 poll()(또는 이와 동등한 함수)를 사용할 필요는 없습니다. 
  
  비차단 파일 기술자를 사용해야 하므로 poll()(또는 이와 동등한 함수)를 사용하지 않고 읽기/쓰기/전송 함수를 사용할 수 있으며 서버가 차단되지 않습니다.
  하지만 시스템 리소스를 더 많이 소모하게 됩니다. 
  
  따라서 poll()(또는 이와 동등한 함수)를 사용하지 않고 파일 디스크립터에서 읽기/받기 또는 쓰기/전송을 시도하면 등급이 0이 됩니다.

• You can use every macro and define like FD_SET, FD_CLR, FD_ISSET, FD_ZERO (understanding what and how they do it is very useful). 
- 모든 매크로를 사용할 수 있으며 FD_SET, FD_CLR, FD_ISSET, FD_ZERO와 같이 정의할 수 있습니다(매크로가 무엇을 어떻게 수행하는지 이해하면 매우 유용합니다). 

• A request to your server should never hang forever. 
- 서버에 대한 요청이 영원히 중단되어서는 안 됩니다. 

• Your server must be compatible with the web browser of your choice. 
- 서버는 선택한 웹 브라우저와 호환되어야 합니다. 

• We will consider that NGINX is HTTP 1.1 compliant and may be used to compare headers and answer behaviors. 
- NGINX는 HTTP 1.1을 준수하며 헤더와 응답 동작을 비교하는 데 사용될 수 있습니다. 


• Your HTTP response status codes must be accurate. 
- HTTP 응답 상태 코드가 정확해야 합니다. 

• You server must have default error pages if none are provided. 
- 기본 오류 페이지가 제공되지 않는 경우 서버에 기본 오류 페이지가 있어야 합니다. 

• You can’t use fork for something else than CGI (like PHP, or Python, and so forth). 
- CGI가 아닌 다른 것(예: PHP, Python 등)에 포크를 사용할 수 없습니다. 

• You must be able to serve a fully static website. 
- 완전한 정적 웹사이트를 제공할 수 있어야 합니다. 

• Clients must be able to upload files. 
- 클라이언트가 파일을 업로드할 수 있어야 합니다. 

• You need at least GET, POST, and DELETE methods. 
- 최소한 GET, POST, DELETE 메서드가 필요합니다. 

• Stress tests your server. It must stay available at all cost. 
- 서버에 대한 스트레스 테스트를 실시합니다. 서버는 항상 가용성을 유지해야 합니다. 


• Your server must be able to listen to multiple ports (see Configuration file).
- 서버가 여러 포트를 수신할 수 있어야 합니다(구성 파일 참조).

# Configuration file
In the configuration file, you should be able to: 

• Choose the port and host of each ’server’. 
- 각 '서버'의 포트와 호스트를 선택합니다. 

• Setup the server_names or not. 
- server_names를 설정할지 여부를 설정합니다. 

• The first server for a host:port will be the default for this host:port (that means it will answer to all the requests that don’t belong to an other server). 
- 호스트:포트의 첫 번째 서버가 이 호스트:포트의 기본값이 됩니다(즉, 다른 서버에 속하지 않은 모든 요청에 응답한다는 의미). 

• Setup default error pages. 
- 기본 오류 페이지를 설정합니다. 

• Limit client body size. 
- 클라이언트 본문 크기를 제한합니다. 

• Setup routes with one or multiple of the following rules/configuration (routes wont be using regexp): 
- 다음 규칙/구성 중 하나 또는 여러 개를 사용하여 라우트를 설정합니다(라우트는 정규식을 사용하지 않음): 

	◦ Define a list of accepted HTTP methods for the route. 
	- 경로에 허용되는 HTTP 메소드 목록을 정의합니다. 

	◦ Define a HTTP redirection. 
	- HTTP 리디렉션을 정의합니다. 

	◦ Define a directory or a file from where the file should be searched (for example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet). 
	- 파일을 검색할 디렉토리 또는 파일을 정의합니다(예: url /kapouet이 /tmp/www에 루팅된 경우, url /kapouet/pouic/toto/pouet은 /tmp/www/pouic/toto/pouet). 

	◦ Turn on or off directory listing.
	- 디렉토리 목록을 켜거나 끕니다.

	◦ Set a default file to answer if the request is a directory. 
	- 요청이 디렉토리인 경우 응답할 기본 파일을 설정합니다. 
	
	◦ Execute CGI based on certain file extension (for example .php). 
	- 특정 파일 확장자(예: .php)를 기준으로 CGI를 실행합니다.
	
	◦ Make it work with POST and GET methods. 
	- POST 및 GET 메소드와 함께 작동하도록 설정합니다. 
	
	◦ Make the route able to accept uploaded files and configure where they should be saved. 
	- 업로드된 파일을 허용하고 저장할 위치를 설정할 수 있는 경로를 만듭니다. 
	
	∗ Do you wonder what a CGI is? 
	- ∗ CGI가 뭔지 궁금하신가요? 
	
	∗ Because you won’t call the CGI directly, use the full path as PATH_INFO. 
	- ∗ CGI를 직접 호출하지 않으므로 전체 경로를 PATH_INFO로 사용합니다. 
	
	∗ Just remember that, for chunked request, your server needs to unchunk it, the CGI will expect EOF as end of the body. 
	- ∗ 청크된 요청의 경우 서버에서 청크를 해제해야 하며, CGI는 본문 끝을 EOF로 예상한다는 점만 기억하세요. 
	
	∗ Same things for the output of the CGI. If no content_length is returned from the CGI, EOF will mark the end of the returned data. 
	- ∗ CGI의 출력도 마찬가지입니다. CGI에서 content_length가 반환되지 않으면 EOF는 반환된 데이터의 끝을 표시합니다. 
	
	∗ Your program should call the CGI with the file requested as first argument. 
	- ∗ 프로그램은 요청된 파일을 첫 번째 인자로 사용하여 CGI를 호출해야 합니다. 
	
	∗ The CGI should be run in the correct directory for relative path file access. 
	- ∗ CGI는 상대 경로 파일 액세스를 위해 올바른 디렉토리에서 실행되어야 합니다. 
	
	∗ Your server should work with one CGI (php-CGI, Python, and so forth). 
	- ∗ 서버는 하나의 CGI(php-CGI, Python 등)로 작동해야 합니다. 

You must provide some configuration files and default basic files to test and demonstrate every feature works during evaluation.
평가 중에 모든 기능이 작동하는지 테스트하고 시연할 수 있도록 몇 가지 구성 파일과 기본 기본 파일을 제공해야 합니다.

If you’ve got a question about one behavior, you should compare your program behavior with NGINX’s. For example, check how does server_name work. We’ve shared with you a small tester. It’s not mandatory to pass it if everything works fine with your browser and tests, but it can help you hunt some bugs. 
한 동작에 대한 질문이 있는 경우 프로그램 동작과 NGINX의 동작을 비교해야 합니다. 예를 들어 server_name이 어떻게 작동하는지 확인하세요. 작은 테스터를 공유해 드렸습니다. 브라우저와 테스트에서 모든 것이 정상적으로 작동한다면 반드시 통과해야 하는 것은 아니지만 버그를 찾는 데 도움이 될 수 있습니다. 


The important thing is resilience. Your server should never die. 
중요한 것은 복원력입니다. 서버가 죽지 않아야 합니다. 


Do not test with only one program. Write your tests with a more convenient language such as Python or Golang, and so forth. Even in C or C++ if you want to.\
하나의 프로그램으로만 테스트하지 마세요. Python이나 Golang 등 더 편리한 언어로 테스트를 작성하세요. 원한다면 C나 C++로도 가능합니다.