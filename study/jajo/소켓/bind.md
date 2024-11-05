#리시프 #socket

```c
#include <sys/types.h>
#include <sys/socket.h>

int bind( SOCKET sockfd, const sockaddr *my_addr, int addrlen );

리턴 값 :
	성공 시 : 0
	에러 시 : -1
```
>소켓에 ip와 포트를 할당한다.

## 매개변수

`int socketfd`
- `connet()` 함수를 호출하고 리턴 받은 socket descriptor
  각각의 소켓을 구분해 주는 값

`const sockaddr* my_addr`
- 실제로 접속하고자 하는 IP와 PORT 정보가 있는 sockaddr 구조체

`socklen_t addrlen`
- sockaddr 구조체의 크기

소켓에는 여러 유형이 있는 만큼 설정하는 방법도 다양하므로 설정을 위한 구조체 타입이 존재한다.
그러므로 bind  함수에는 type casting하여 타입을 넘겨줘야 한다.
다만 컴퓨터는 해당 매개변수의 크기를 알 수 없으므로 3번째 매개변수를 통해 2번째 입력된 데이터의 크기를 전달해야 한다.

널리 쓰이는 IPv4 도메인에서 통신하기 위한 소켓을 bind 할때는 sockaddr_in 타입이 주로 쓰이며 내부 통신용 소켓의 bind에는 sockaddr_in이 쓰인다. 각 구조체의 원형은 아래와 같다.

```c
struct sockaddr {
   unsigned short  sa_family;   
   char    sa_data[14];
};

struct sockaddr_in {
   short   sin_family;
   unsigned short sin_port;
   struct  in_addr sin_addr;
   char    sin_zero[8];
};

struct sockaddr_un {
    unsigned short sin_family;
    char sun_path[108;
};
```

위 구조체를 보면 sa_family와 sin_family의 타입이 똑같은것을 볼 수 있는데,  이 변수값을 통해 bind 함수가 전달받은 소켓의 종류를 알 수 있다.
## 예제
```c 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/socket.h> 

#define PORT 1212 

int main() { 
	struct sockaddr_in clientaddr, serveraddr, myaddr;
	
	// 사용전에 초기화
	memset(&serveraddr, 0x00, sizeof(serveraddr));    
	
	serveraddr.sin_family = AF_INET;
	
	// 모든 주소에서 받겠다(0.0.0.0)
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	
	serveraddr.sin_port = htons(12345);
	state = bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
	
	if (state == -1) {
	    perror("bind error : ");
	    exit(0);
	}
}
```