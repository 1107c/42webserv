#리시프 #socket

```c
#include <sys/types.h>
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

리턴 값 :
	성공 시 : 0
	에러 시 : <0
```
>연결하고자 하는 단말(서버 등)에 연결해 주는 함수

## 매개변수

`int socketfd`
- `connet()` 함수를 호출하고 리턴 받은 socket descriptor
  각각의 소켓을 구분해 주는 값

`const struct sockaddr* addr`
- 실제로 접속하고자 하는 IP와 PORT 정보가 있는 sockaddr 구조체

`socklen_t addrlen`
- sockaddr 구조체의 크기


## 예제
```c 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/socket.h> 

#define PORT 1212 

int main() { 
	int sock; 
	struct sockaddr_in addr; 
	
	if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{ 
		perror("socket"); exit(1); 
	} 
	
	// sockaddr_in 구조체 초기화. 
	memset(&addr, 0x00, sizeof(addr)); 
	
	// 주소 체계 지정. 
	addr.sin_family = AF_INET; 
	
	// 연결할 단말의 ip 지정(127.0.0.1은 루프백 주소) 
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	
	// 어떤 PORT를 사용할지 지정 
	addr.sin_port = htons(PORT);
	
	// connect() 함수를 호출한 후 연결에 성공하면 0을 리턴 
	// 실패하면 0보다 작은 값을 리턴 
	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{ 
		perror("connect"); exit(1); 
	} 
}
```