>`epoll` 인스턴스에 파일 디스크립터를 추가, 수정 또는 삭제합니다
	이 함수는 이벤트 기반 프로그래밍에서 매우 중요한 역할을 하며, 특정 파일 디스크립터에 대해 어떤 이벤트를 감시할 것인지 설정합니다.

# 헤더
```c
#include <sys/epoll.h>

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

```

- **epfd**: `epoll` 인스턴스의 파일 디스크립터입니다. `epoll_create` 또는 `epoll_create1` 호출로 생성된 파일 디스크립터입니다.

- **op**: 수행할 작업을 지정하는 상수입니다. 사용할 수 있는 값은 다음과 같습니다:
    - **`EPOLL_CTL_ADD`**: 새로운 파일 디스크립터를 추가합니다.
    - **`EPOLL_CTL_MOD`**: 이미 추가된 파일 디스크립터의 감시 이벤트를 수정합니다.
    - **`EPOLL_CTL_DEL`**: 파일 디스크립터를 삭제합니다.

- **fd**: 작업을 수행할 파일 디스크립터입니다. 일반적으로 소켓이나 파일입니다.

- **event**[[epoll_event 구조체]]: 감시할 이벤트 정보를 담고 있는 구조체입니다. 이 구조체는 `epoll_event` 타입입니다.
	- **`EPOLLIN`**: 읽기 가능
	- **`EPOLLOUT`**: 쓰기 가능
	- **`EPOLLERR`**: 오류 발생
	- **`EPOLLHUP`**: 연결 종료
	- **`EPOLLET`**: 엣지 트리거 모드

- **data**: 사용자 정의 데이터를 저장할 수 있습니다. 일반적으로 파일 디스크립터나 포인터를 사용합니다.




