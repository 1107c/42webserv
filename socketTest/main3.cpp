#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unordered_map>
#include <fcntl.h>
#include <signal.h>
#include <algorithm>
#include <cstring>
#include <sys/epoll.h>
#include <vector>

volatile sig_atomic_t running = 1;

void signal_handler(int signum) {
    if (signum == SIGINT) {
        running = 0;
    }
}

std::string create_http_response() {
    std::ifstream file("home.html");
    std::string html;
    std::string line;
    while (std::getline(file, line)) {
        html += line + "\n";
    }
    std::ostringstream ss;
    ss << html.size();
    
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: " + ss.str() + "\r\n";
    response += "\r\n";
    response += html;
    return response;
}

int main() {	
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);					//AF_INET << IPv4 - SOCK_STREAM << TCP
    if (listen_sock == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    int flags = fcntl(listen_sock, F_GETFL, 0);							//	첫번째 인자 대상 F_GETFL << 현재설정 가져오기 
    fcntl(listen_sock, F_SETFL, flags | O_NONBLOCK);					// 첫번째 대상 F_SETFL << SET | O_NONBLOCK 기능만 추가

    int opt = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {				// 재사용을 위한 SO_REUSEADDR 예상치못한 종료에 의해서
        std::cerr << "setsockopt failed" << std::endl;
        return 1;
    }

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;			// 주소 대응
    address.sin_port = htons(8080);					// AF_INET IPv4  INADDR_ANY  port == 사용할 포트 입력 htons

    if (bind(listen_sock, (struct sockaddr *)&address, sizeof(address)) < 0) {				// 바인딩 입혀준다 소켓에
        std::cerr << "Bind failed" << std::endl;
        return 1;
    }

    if (listen(listen_sock, SOMAXCONN) < 0) {				//wait 2번째 인자가 최대 입력가능 수
        std::cerr << "Listen failed" << std::endl;
        return 1;
    }

	int epoll_fd = epoll_create1(EPOLL_CLOEXEC);		//epoll_create(1024)          epoll_create1(EPOLL_CLOEXEC)  fork exec  자동으로 해지
	if (epoll_fd == -1) {
		std::cerr << "epoll_create1 failed" << std::endl;
		return 1;
	}

	std::unordered_map<int, struct epoll_event> evs;		//Server_fd client_fd   bucket
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {			//epoll_ctl 첫번째 인자 epoll 관리자 두번째 ADD DEL MOD 3번째인자가 해당하는 디스크립터 DEL는 4번째인자 무시
		std::cerr << "epoll_ctl failed for listen socket" << std::endl;
		return 1;
	}
	evs[ev.data.fd] = ev;

	const int MAX_EVENTS = 100;
	std::vector<struct epoll_event> events(MAX_EVENTS);

	signal(SIGINT, signal_handler);
	while (running) {
		int event_count = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);			//첫번째 인자의 디스크립터가 대기, 두번째 내용, -1 블로킹
		if (event_count == -1) {
			if (errno == EINTR) continue ;
			std::cerr << "epoll_wait failed" << std::endl;
			break ;
		}

		for (int i = 0; i < event_count; ++i) {
			if (events[i].data.fd == listen_sock) {				// 신규 클라이언트 생성부분
				struct sockaddr_in client_addr;
				socklen_t client_len = sizeof(client_addr);
				int clientSock = accept(listen_sock, (struct sockaddr*)&client_addr, &client_len); //연결 부분 
				if (clientSock < 0) {
					std::cerr << "Accept failed" << std::endl;
					continue ;
				}

				int flags = fcntl(clientSock, F_GETFL, 0);
				fcntl(clientSock, F_SETFL, flags | O_NONBLOCK);

				struct epoll_event clientEv;
				clientEv.events = EPOLLIN;
				clientEv.data.fd = clientSock;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSock, &clientEv) == -1) {
					std::cerr << "epoll_ctl failed for client socket" << std::endl;
					close(clientSock);
					continue ;
				}
				evs[clientSock] = clientEv;
			} else if (events[i].events & EPOLLIN) {
				char buffer[8192];
				int bytes_read = recv(events[i].data.fd, buffer, sizeof(buffer), 0);

				if (bytes_read > 0) {
					std::string result = std::string(buffer, bytes_read);
					if (result.find("favicon") == std::string::npos)
						std::cout << "Received: " << std::string(buffer, bytes_read) << std::endl;
					evs[events[i].data.fd].events |= EPOLLOUT;
					epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[i].data.fd, &evs[events[i].data.fd]);
				} else if (bytes_read == 0) {
					evs.erase(events[i].data.fd);
					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
					close(events[i].data.fd);
				}
			} else if (events[i].events & EPOLLOUT) {
				static std::unordered_map<int, std::string> pending_responses;
				
				if (pending_responses.find(events[i].data.fd) == pending_responses.end()) {
					pending_responses[events[i].data.fd] = create_http_response();
				}

				std::string& response = pending_responses[events[i].data.fd];
				int bytes_sent = send(events[i].data.fd, response.c_str(), response.size(), 0);

				if (bytes_sent > 0) {
					response = response.substr(bytes_sent);

					if (response.empty()) {
						pending_responses.erase(events[i].data.fd);
						evs[events[i].data.fd].events &= ~EPOLLOUT;
						epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[i].data.fd, &evs[events[i].data.fd]);
					}
				} else if (bytes_sent == -1) {
					std::cerr << "Send failed" << std::endl;
				}
			}
		}
	}

	for (const auto& pair : evs) {
		if (pair.first != listen_sock) {
			epoll_ctl(epoll_fd, EPOLL_CTL_DEL, pair.first, NULL);
			close(pair.first);
		}
	}

	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, listen_sock, NULL);
	close(listen_sock);
	close(epoll_fd);
}