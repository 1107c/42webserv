> `epoll`은 리눅스에서 **대규모 파일 디스크립터 집합에 대해 효율적인 I/O 이벤트를 모니터링**하는 시스템 호출입니다. 


 서버 프로그래밍에서 다수의 클라이언트 연결을 동시에 처리할 때, `epoll`을 사용하면 리소스를 적게 소모하면서도 빠르게 처리할 수 있습니다. 
 `epoll`은 `select`나 `poll`에 비해 더 나은 성능을 제공하며, 비동기 I/O 작업을 처리하는 데 특히 효과적입니다.

# 주요 함수
1. [[epoll_create]]
2. [[epoll_ctl]]
3. [[epoll_wait]]

# 예제 : epoll을 사용한 서버 구현
다음 예제는 `epoll`을 사용하여 다수의 클라이언트 연결을 비동기적으로 처리하는 간단한 TCP 서버입니다. 클라이언트가 연결하면 `epoll`을 통해 연결 소켓을 모니터링하고, 데이터가 도착하면 수신합니다.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <errno.h>

#define MAX_EVENTS 10
#define PORT 8080

int main() {
    int server_fd, epoll_fd, new_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    struct epoll_event ev, events[MAX_EVENTS];

    // 서버 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 소켓 옵션 설정
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 소켓 바인딩
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 소켓 리슨 설정
    if (listen(server_fd, 5) == -1) {
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

    // 서버 소켓을 epoll에 등록
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        perror("epoll_ctl: server_fd failed");
        close(server_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        // 이벤트 기다리기
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait failed");
            close(server_fd);
            close(epoll_fd);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                // 새로운 클라이언트 연결 수락
                new_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
                if (new_fd == -1) {
                    perror("accept failed");
                    continue;
                }

                // 새로운 클라이언트 소켓을 epoll에 등록
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = new_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_fd, &ev) == -1) {
                    perror("epoll_ctl: new_fd failed");
                    close(new_fd);
                    continue;
                }

                printf("Accepted new connection\n");
            } else {
                // 클라이언트로부터 데이터 수신
                char buffer[1024];
                ssize_t n = read(events[i].data.fd, buffer, sizeof(buffer) - 1);
                if (n == -1) {
                    perror("read failed");
                    close(events[i].data.fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                } else if (n == 0) {
                    // 클라이언트 연결 종료
                    printf("Client disconnected\n");
                    close(events[i].data.fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                } else {
                    buffer[n] = '\0';
                    printf("Received: %s\n", buffer);
                    // 클라이언트에게 에코
                    write(events[i].data.fd, buffer, n);
                }
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}
```

### 코드 설명

1. **서버 소켓 생성**: `socket`을 통해 TCP 소켓을 생성하고, `bind`와 `listen`으로 서버 설정을 완료합니다.
2. **`epoll` 인스턴스 생성**: `epoll_create1`을 사용하여 `epoll` 인스턴스를 생성합니다.
3. **서버 소켓을 `epoll`에 등록**: 서버 소켓 파일 디스크립터를 `EPOLLIN` 이벤트와 함께 `epoll`에 추가합니다.
4. **이벤트 루프**:
    - `epoll_wait`로 이벤트가 발생할 때까지 대기합니다.
    - `events` 배열에서 각 이벤트를 확인합니다. 만약 서버 소켓에서 이벤트가 발생하면 새로운 연결을 수락하고, 해당 클라이언트 소켓을 `epoll`에 추가합니다.
    - 클라이언트 소켓에서 읽기 이벤트가 발생하면 데이터를 수신하고, 수신한 데이터를 그대로 클라이언트에 다시 전송합니다(에코).
5. **클라이언트 연결 종료 처리**: 클라이언트가 연결을 종료하면 해당 소켓을 닫고 `epoll`에서 제거합니다.

### 주요 옵션 설명

- **EPOLLIN**: 읽기 가능한 데이터가 있는 경우 이벤트가 발생합니다.
- **EPOLLET**: 엣지 트리거 모드(Edge Triggered)로, 데이터가 처음 도착할 때만 이벤트가 발생합니다.