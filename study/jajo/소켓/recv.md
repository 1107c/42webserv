#리시프 #socket

```c
#include <sys/types.h>
#include <sys/socket.h>

int recv(int socketfd, void *buf, size_t len, int flags);

리턴 값 :
	성공 시 : 실제 수신한 바이트 수
	에러 시 : -1
```
>연결된 서버나 클라이언트로 데이터를 전송한다.
## 매개변수

`int socketfd`
- `connet()` 함수를 호출하고 리턴 받은 socket descriptor
  각각의 소켓을 구분해 주는 값

`void *msg`
- 수신할 데이터

`size_t len`
- 데이터의 바이트 단위 길이

`int flags`
- MSG_DONTWAIT : 전송할 준비가 전에 대기 상태가 필요하다면 기다리지 않고 -1을 반환하면서 복귀
- MSG_NOSIGNAL : 상대방과 연결이 끊겼을 때, SIGPIPE 시그널을 받지 않도록 한다.
