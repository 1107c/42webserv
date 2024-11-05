>리눅스의 `epoll` 메커니즘에서 **이벤트를 대기하고 감지하는 함수**입니다. 
>
>이 함수는 특정 파일 디스크립터에서 발생한 이벤트를 확인하고, 해당 이벤트가 발생할 때까지 블로킹하거나 지정된 시간 동안 대기합니다.
>
 `epoll_wait`는 비동기 I/O 모델에서 핵심적인 역할을 하며, 성능이 뛰어난 이벤트 기반 프로그래밍을 가능하게 합니다.

# 헤더
```c
#include <sys/epoll.h>

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

발생한 이벤트의 수를 반환, 실패시 -1을 반환하고 errno 설정
```

- **epfd**: `epoll_create` 또는 `epoll_create1`로 생성된 `epoll` 인스턴스의 파일 디스크립터입니다.

- **events**: 발생한 이벤트가 저장될 배열입니다. 배열의 각 요소는 `epoll_event` 구조체로, 이벤트 정보가 포함됩니다.

- **maxevents**: `events` 배열의 크기로, 동시에 수신할 수 있는 최대 이벤트 수입니다.

- **timeout**: 대기 시간 (밀리초)입니다. 이 값이 `-1`이면 무한 대기, `0`이면 즉시 반환, 양수일 경우 지정된 밀리초만큼 대기합니다.

