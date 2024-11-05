> `epoll` 인스턴스를 생성하는 함수입니다. 
> 이 함수는 파일 디스크립터를 모니터링할 수 있는 `epoll` 객체를 만들어, 여러 개의 파일 디스크립터에 대한 I/O 이벤트를 효율적으로 관리할 수 있도록 합니다

하지만 `epoll_create`는 **현재 더 이상 권장되지 않으며** 대신 **`epoll_create1`**을 사용하는 것이 일반적입니다. 

`epoll_create`는 리눅스 커널 2.6부터 사용되었지만, 이후 리눅스 커널이 업데이트되면서 `epoll_create1`이 등장했고, `epoll_create`의 기능을 대체합니다. 
`epoll_create1`은 `epoll` 인스턴스 생성 시 추가 옵션(`EPOLL_CLOEXEC`)을 제공하여 **보다 유연하게 사용**할 수 있도록 해줍니다.

# 헤더
```c
#include <sys/epoll.h>

int epoll_create(int size);

성공시 epoll 인스턴스의 fd, 실패시 -1 리턴
```
- **`size`**: 감시할 파일 디스크립터의 개수를 지정하는 인자로, 초기 크기 힌트로만 사용됩니다. 
  이 인자는 리눅스 커널 2.6.8 이후 무시되지만, `0`보다 큰 값을 넣어야 합니다.

이 함수는 `epoll` 인스턴스(파일 디스크립터)를 생성하는 데 사용되며, 반환된 파일 디스크립터는 `epoll_ctl` 및 `epoll_wait`에서 사용됩니다. 
생성된 `epoll` 인스턴스는 `close`를 사용하여 해제할 수 있습니다.

> **참고:** `epoll_create1`는 `int epoll_create1(int flags);` 
> 형태로, `flags` 인자를 통해 `EPOLL_CLOEXEC` 옵션을 설정할 수 있어 더 유연합니다.

# 예제
다음 예제에서는 `epoll_create`를 사용하여 `epoll` 인스턴스를 생성하고, 기본 TCP 서버에서 소켓을 등록하여 다수의 클라이언트 연결을 처리하는 코드를 보여줍니다.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

int main() {
    int epfd;
    struct epoll_event ev, events[1]; // 이벤트 구조체

    // epoll 인스턴스 생성 (size는 무시되지만 양수여야 함)
    epfd = epoll_create(1);
    if (epfd == -1) {
        perror("epoll_create failed");
        exit(EXIT_FAILURE);
    }

    // 표준 입력(STDIN_FILENO)을 감시 대상으로 추가
    ev.events = EPOLLIN;     // 읽기 이벤트 감지
    ev.data.fd = STDIN_FILENO; // 표준 입력 파일 디스크립터
    
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1) {
        perror("epoll_ctl failed");
        close(epfd);
        exit(EXIT_FAILURE);
    }

    printf("Type something and press enter...\n");

    // 이벤트 대기
    while (1) {
        int nfds = epoll_wait(epfd, events, 1, -1); // 무한 대기
        if (nfds == -1) {
            perror("epoll_wait failed");
            close(epfd);
            exit(EXIT_FAILURE);
        }

        if (events[0].data.fd == STDIN_FILENO) {
            // 표준 입력으로부터 데이터가 도착하면 읽기
            char buffer[100];
            ssize_t n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
            if (n == -1) {
                perror("read failed");
                close(epfd);
                exit(EXIT_FAILURE);
            }
            buffer[n] = '\0';
            printf("You typed: %s", buffer);
        }
    }

    close(epfd);
    return 0;
}
```
### 코드 설명

1. **`epoll` 인스턴스 생성**: `epoll_create(1)`을 호출하여 `epoll` 인스턴스를 생성합니다. 반환된 파일 디스크립터 `epfd`는 이후 `epoll` 관련 작업에서 사용됩니다.

2. **표준 입력 감시 등록**: `epoll_ctl`로 `STDIN_FILENO`를 감시 대상으로 등록하고, `EPOLLIN` 플래그를 설정하여 **읽기 이벤트**가 발생할 때 알림을 받도록 합니다.

3. **이벤트 대기**: `epoll_wait`로 이벤트가 발생할 때까지 무한 대기합니다. 이벤트가 발생하면 `events` 배열에 이벤트가 저장됩니다.

4. **이벤트 처리**: `STDIN_FILENO`에서 데이터를 읽고, 사용자가 입력한 내용을 출력합니다.

이 예제는 `epoll_create`와 `epoll_ctl`, `epoll_wait`을 사용하여 간단한 이벤트 기반 입력 감지 프로그램을 구현한 것입니다.