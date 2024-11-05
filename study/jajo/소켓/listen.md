#리시프 #socket

```c
#include <sys/types.h>
#include <sys/socket.h>

int listen(int sockfd, int backlog);

리턴 값 :
	성공 시 : 0
	에러 시 : -1
```
>클라이언트의 요청에 대한 처리가 끝나기 전에 새로운 클라이언트가 요청 시 대기열을 생성하기 위한 함수 (큐잉)
## 매개변수

`int socketfd`
- `connet()` 함수를 호출하고 리턴 받은 socket descriptor
  각각의 소켓을 구분해 주는 값

`int backlog`
- 수신 대기열의 크기
