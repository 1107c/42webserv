#리시프 #socket

```c
#include <sys/types.h>
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

리턴 값 :
	성공 시 : >= 0, 소켓 지정 번호
	에러 시 : -1
```
>수신 대기열의 가장 앞에 있는 연결 요청을 가져온다.
>연결 요청을 성공적으로 가져오면 클라이언트와의 통신을 위한 연결 소켓을 생성한다.

## 매개변수

`int socketfd`
- `connet()` 함수를 호출하고 리턴 받은 socket descriptor
  각각의 소켓을 구분해 주는 값

`struct sockaddr* addr`
- accept 함수 성공 시, 연결된 client 의 주소와 포트 정보가 담기는 구조체

`socklen_t addrlen`
- sockaddr 구조체의 크기

함수 자체 컨셉은 bind 함수와 유사하다.
반환 값은 socket 함수로 만든 듣기 소켓과는 전혀 다른 별개의 소켓이다.

듣기 소켓
- 연결 요청을 확인하는 일만 수행, 연결 요청을 받으면 즉시 수신 대기열로 넘겨 다음 요청을 기다린다.
연결 소켓
- 실제 client 와 통신하는 소켓

## 예제
```c 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/socket.h> 

#define PORT 1212 

int main() { 
	struct sockaddr_in, client_sockaddr;
	
	int client_sockfd;
	int client_addr_size;
	
	while(1){
	    client_addr_size = sizeof(client_addr);
	    client_sockfd = accept(sockfd,
	        (struct sockaddr *) &client_sockaddr,
	        &client_addr_size
	    );
	
	    if (client_sockfd == -1) {
	        // 에러 처리
	    }
	    else {
	        // client_sockfd를 이용해서 통신을 한다.
	    }
	}
}
```