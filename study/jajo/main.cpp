//socket, 매개변수
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

//에러처리 출
#include <iostream>

//close
#include <unistd.h>

//memset
#include <cstring>
//bind
#include <sys/types.h>

//epoll
#include <sys/epoll.h>
#include <unordered_map>

//signal
#include <signal.h>

//vector
#include <map>
#include <vector>

volatile sig_atomic_t running = 1;

void signal_handler(int signum) {
	if (signum == SIGINT) {
		running = 0;
	}
}

int plusNonBlockingMode(int *fd)
{
	/* 현재 파일 플래그 가져오기 */
	int flags = fcntl(*fd, F_GETFL);
	if (flags == -1) {
		std::cerr << "Failed to get file flags\n";
		close(*fd);
		return (1);
	}

	/* non-blocking 모드 추가 */
	if (fcntl(*fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		std::cerr << "Failed ot set file to non-blocking mode\n";
		close(*fd);
		return (1);
	}
	return (0);
}

int setSockOpt(int *fd)
{
	int opt = 1;
	if (setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		std::cerr << "setsockopt failed\n";
		close(*fd);
		return (1);
	}
	return (0);
}

int setSockBindListen(sockaddr_in *serverAddr, int fd)
{
	memset(serverAddr, 0, sizeof(*serverAddr));
	serverAddr->sin_family = AF_INET;
	serverAddr->sin_addr.s_addr = INADDR_ANY;
	serverAddr->sin_port = htons(8080);

	//소켓 바인
	if (bind(fd, (struct sockaddr *)serverAddr, sizeof(*serverAddr)) == -1) {
		std::cerr << "bind failed\n";
		close(fd);
		return (1);
	}

	//클라이언트의 연결 요청 대
	if (listen(fd, 5) == -1) {
		std::cerr << "listen failed\n";
		close(fd);
		return (1);
	}
	return (0);
}

int main(void)
{
	int serverSocket;

	/* 서버 소켓 생성 */
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_HOPOPTS);
	if (serverSocket == 0) {
		std::cerr << "소켓 할당 실패\n";
		return (1);
	}

	/* 서버 소켓 non-blocking 모드 추가 및 reuseaddr 옵션 활성화 */
	if (plusNonBlockingMode(&serverSocket) || setSockOpt(&serverSocket)) {
		return (1);
	}

	/* 서버 주소 설정 */
	struct sockaddr_in serverAddr;
	if (setSockBindListen(&serverAddr, serverSocket)) {
		return (1);
	}


	/* epoll 생성 */
	int epollFd;
	struct epoll_event ev;
	epollFd = epoll_create(1);
	if (epollFd == -1) {
		std::cerr << "epoll_create failed\n";
		return (1);
	}
	std::map<int, struct epoll_event> evs; //server, client

	//서버 소켓을 epoll에 등록
	ev.events = EPOLLIN;
	ev.data.fd = serverSocket;
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &ev) == -1) {
		std::cerr << "epoll_ctl failed for server socket\n";
		return (1);
	}

	evs[ev.data.fd] = ev; //이부분 잘 이해안감

	const int MAX_EVENTS = 100;
	//이벤트가 저장될 배열
	std::vector<struct epoll_event> events(MAX_EVENTS);

	signal(SIGINT, signal_handler);
	while(running) {
		//event waiting
		//nfds 발생한 이벤트 수
		int nfds = epoll_wait(epollFd, events.data(), MAX_EVENTS, -1); // -1이면 무한 대기
		if (nfds == -1) {
			std::cerr << "epoll_wait failed\n";
			close(serverSocket);
			close(epollFd);
			break;
		}

		for (int i = 0; i < nfds; i++) {
			//신규 클라이언트 생성부분
			if (events[i].data.fd == serverSocket) {
				//새로운 클라이언트 연결 수락
				struct sockaddr_in clientAddr;
				socklen_t clientLen = sizeof(clientAddr);

				int clientSock = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
				if (clientSock < 0) {
					std::cerr <<"Accept failed\n";
					continue;
				}

				//비동기 모드 추가
				plusNonBlockingMode(&clientSock);

				//새로운 클라이언트 소켓을 epoll에 등록
				struct epoll_event clientEv;
				//이벤트 등록해 이후 데이터를 수신할 수 있도록 준비
				clientEv.events = EPOLLIN;
				clientEv.data.fd = clientSock;
				if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSock, &clientEv) == -1) {
					std::cerr << "epoll_ctl failed for client socket\n";
					close(clientSock);
					continue;
				}
				evs[clientSock] = clientEv;
			} 
			/* 데이터 수신 요청 처리 */
			//데이터 수신하고, 수신된 데이터가 있을 경우, 해당 소켓을 EPOLLOUT 상태로 변경해 응답을 준비한다.
			//클라이언트가 연결을 종료하면 소켓을 epoll에서 제거하고 닫는다.
			else if (events[i].events & EPOLLIN) {
				char buffer[8192];
				int bytesRead = recv(events[i].data.fd, buffer, sizeof(buffer), 0);

				if (bytesRead > 0) {
					std::string result = std::string(buffer, bytesRead);
					if (result.find("favicon") == std::string::npos)
						std::cout << "Received: " << std::string(buffer, bytesRead) << std::endl;
					evs[events[i].data.fd].events |= EPOLLOUT;
					epoll_ctl(epollFd, EPOLL_CTL_MOD, events[i].data.fd, &evs[events[i].data.fd]);
				} else if (bytesRead == 0) {
					evs.erase(events[i].data.fd);
					epoll_ctl(epollFd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
					close(events[i].data.fd);
				}
			}
			/* 데이터 전송 처리 */
			else if (events[i].events & EPOLLOUT) {
				static std::map<int, std::string> pendingResponses;
				if (pendingResponses.find(events[i].data.fd) == pendingResponses.end()) {
					//pendingResponses[events[i].data.fd] = create_http_response();
				}
				std::string& response = pendingResponses[events[i].data.fd];
				int bytesSent = send(events[i].data.fd, response.c_str(), response.size(), 0);

				if (bytesSent > 0) {
					response = response.substr(bytesSent);
					if (response.empty()) {
						pendingResponses.erase(events[i].data.fd);
						evs[events[i].data.fd].events &= ~EPOLLOUT;
						epoll_ctl(epollFd, EPOLL_CTL_MOD, events[i].data.fd, &evs[events[i].data.fd]);
					}
				} else if (bytesSent == -1) {
					std::cerr << "send Failed\n";
				}
			}
		}
	}
	epoll_ctl(epollFd, EPOLL_CTL_DEL, serverSocket, NULL);
	close(serverSocket);
	close(epollFd);
}
