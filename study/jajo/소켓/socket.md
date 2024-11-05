#리시프 #socket

```c
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);

리턴 값 :
	성공 시 : socket descriptor
	에러 시 : <0
```
>소켓 생성 함수

## 매개변수

`int domain`
- 통신 도메인을 지정하는 인자
- 어떤 네트워크에서 사용될 소켓인지 지정
- PF(Protocol Family, 프로토콜 체계), AF_INET(Address Family, 주소체계)가 들어간다.
	- PF_INET : IPv4 프로토콜
	- PF_INET6 : IPv6 프로토콜
	- PF_LOCAL : LOCAL 통신을 위한 UNIX 프로토콜
	- PF_PACKET : Low Level Socket을 위한 인터페이스
	- PF_IPX : IPF 노벨 프로토콜
	- AF_INET : IPv4 프로토콜
	- AF_INET6 : IPv6 프로토콜
	- AF_LOCAL : LOCAL통신을 위한 UNIX 프로토콜

`int type`
- 소켓의 형태를 지정한다.
	- SOCK_STREAM : 스트림을 의미, TCP 소켓(연결지향형 소켓)을 할당함
	- SOCK_DGRAM : 데이터 그램을 의미, UDP 소켓(비연결지향형 소켓)을 할당함
	- SOCK_RAW : RAW 소켓 생성, 프로토콜의 헤더를 직접 다룰 때 사용
		- 프로토콜의 헤더를 직접 만들 수 있고 송수신하는 패킷들의 데이터를 볼 수 있기 때문에 보안 상의 이유로 root권한으로 실행시켜야 한다.

`int protocol`
- 실제로 사용할 통신 프로토콜을 지정한다.
	- IPPROTO_TCP : TCP 프로토콜을 사용함
	- IPPROTO_UDP : UDP 프로토콜을 사용함
	- IPPROTO_HOPOPTS : 첫 번째, 두 번째 매개변수를 기준으로 자동지정