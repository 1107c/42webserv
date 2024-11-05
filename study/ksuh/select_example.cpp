#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>

int main() {
	// 서버 소켓 설정
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0) {
		std::cerr << "소켓 생성 실패\n";
		return -1;
	}

	sockaddr_in server_addr{};
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8080);

	// 서버 소켓 바인딩
	if (bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "바인딩 실패\n";
		return -1;
	}

	// 서버 소켓 리스닝
	if (listen(server_sock, 5) < 0) {
		std::cerr << "리스닝 실패\n";
		return -1;
	}
	std::cout << "서버가 포트 8080에서 리스닝 중...\n";

	fd_set master_set, read_set;
	FD_ZERO(&master_set);
	FD_SET(server_sock, &master_set);
	int max_fd = server_sock;

	while (true) {
		read_set = master_set;

		// select 호출
		int activity = select(max_fd + 1, &read_set, nullptr, nullptr, nullptr);
		if (activity < 0) {
			std::cerr << "select 오류\n";
			break;
		}

		// 서버 소켓에서 새로운 연결 요청 처리
		if (FD_ISSET(server_sock, &read_set)) {
			int client_sock = accept(server_sock, nullptr, nullptr);
			if (client_sock < 0) {
				std::cerr << "클라이언트 연결 실패\n";
				continue;
			}
			std::cout << "새로운 클라이언트 연결: " << client_sock << "\n";
			FD_SET(client_sock, &master_set);
			if (client_sock > max_fd) max_fd = client_sock;
		}

		// 클라이언트 소켓에서 데이터 읽기
		for (int i = 0; i <= max_fd; ++i) {
			if (i != server_sock && FD_ISSET(i, &read_set)) {
				char buffer[1024];
				int bytes_read = recv(i, buffer, sizeof(buffer) - 1, 0);
				if (bytes_read <= 0) {
					std::cout << "클라이언트 연결 종료: " << i << "\n";
					close(i);
					FD_CLR(i, &master_set);
				} else {
					buffer[bytes_read] = '\0';
					std::cout << "클라이언트 " << i << "로부터 받은 메시지: " << buffer << "\n";
				}
			}
		}
	}

	close(server_sock);
	return 0;
}
