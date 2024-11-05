#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024
#define PORT 8080

int main() {
	int server_fd, new_socket, epoll_fd;
	struct sockaddr_in address;
	char buffer[BUFFER_SIZE];
	struct epoll_event ev, events[MAX_EVENTS];

	// 서버 소켓 생성
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// 서버 주소 설정
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// 소켓 바인딩
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("bind failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	// 소켓을 수신 대기 상태로 설정
	if (listen(server_fd, 3) < 0) {
		perror("listen failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	// epoll 인스턴스 생성
	epoll_fd = epoll_create1(0);
	if (epoll_fd == -1) {
		perror("epoll_create1 failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	// 서버 소켓을 epoll에 추가
	ev.events = EPOLLIN;  // 읽기 가능한 이벤트
	ev.data.fd = server_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
		perror("epoll_ctl: server_fd");
		close(server_fd);
		close(epoll_fd);
		exit(EXIT_FAILURE);
	}

	std::cout << "Server listening on port " << PORT << std::endl;

	// 이벤트 루프
	while (true) {
		int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			perror("epoll_wait failed");
			close(server_fd);
			close(epoll_fd);
			exit(EXIT_FAILURE);
		}

		for (int n = 0; n < nfds; ++n) {
			if (events[n].data.fd == server_fd) {
				// 새로운 클라이언트 접속
				struct sockaddr_in client_address;
				socklen_t client_addr_len = sizeof(client_address);
				new_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_addr_len);
				if (new_socket == -1) {
					perror("accept failed");
					continue;
				}

				// 새 클라이언트 소켓을 epoll에 추가
				ev.events = EPOLLIN;
				ev.data.fd = new_socket;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &ev) == -1) {
					perror("epoll_ctl: new_socket");
					close(new_socket);
				}

				std::cout << "Accepted connection from "
						  << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl;
			} else {
				// 기존 클라이언트로부터 데이터 수신
				int client_fd = events[n].data.fd;
				int bytes_read = read(client_fd, buffer, BUFFER_SIZE);
				if (bytes_read <= 0) {
					// 클라이언트 연결이 종료됨
					if (bytes_read == 0) {
						std::cout << "Client disconnected" << std::endl;
					} else {
						perror("read failed");
					}
					close(client_fd);
					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL); // epoll에서 제거
				} else {
					// 받은 데이터를 다시 클라이언트에 에코
					buffer[bytes_read] = '\0';
					std::cout << "Received: " << buffer << std::endl;
					send(client_fd, buffer, bytes_read, 0);
				}
			}
		}
	}

	close(server_fd);
	close(epoll_fd);
	return 0;
}