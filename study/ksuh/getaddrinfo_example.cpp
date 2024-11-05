#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>


// addrinfo 구조체 형식

// struct addrinfo {
// 	int				ai_flags;		// 옵션 플래그 (주소 설정 방식 등)
// 	int				ai_family;	 	// 주소 패밀리 (AF_INET, AF_INET6, AF_UNSPEC 등)
// 	int				ai_socktype;	// 소켓 타입 (SOCK_STREAM, SOCK_DGRAM 등)
// 	int				ai_protocol;	// 프로토콜 (IPPROTO_TCP, IPPROTO_UDP 등)
// 	socklen_t		ai_addrlen;		// 소켓 주소의 길이
// 	struct sockaddr *ai_addr;		// 소켓 주소를 가리키는 포인터
// 	char			*ai_canonname;	// 공식 도메인 이름
// 	struct addrinfo *ai_next;		// 다음 addrinfo 구조체를 가리키는 포인터 (링크드 리스트 형태)
// };


// getaddrinfo, freeaddrinfo, gai_strerror는 네트워크 프로그래밍에서 
// DNS 조회 및 소켓 주소 처리를 도와주는 함수들입니다. 
// 이 함수들은 네트워크 주소와 관련된 정보를 얻고 관리하는데 사용됩니다.

// getaddrinfo: 호스트 이름과 서비스 이름을 IP 주소와 포트 번호로 변환합니다.
// freeaddrinfo: getaddrinfo로 할당된 메모리를 해제합니다.
// gai_strerror: getaddrinfo가 반환한 오류 코드를 문자열로 변환합니다.


int main ( int argc, char *argv[] ) 
{ 
	int status; 
	struct addrinfo hints; 
	struct addrinfo *res, *p; // 결과를 저장할 변수 

	if ( !argv[1] )
		return 1;

	memset( &hints, 0, sizeof(hints) );
	hints.ai_family = AF_INET; 			// IPv4
	hints.ai_socktype = SOCK_STREAM; 	// TCP stream sockets 
	hints.ai_flags = AI_CANONNAME; 		// 공식 도메인 이름을 요청
	
	status = getaddrinfo( argv[1], NULL, &hints, &res ); // status: 0 on success
	std::cout << "status: " << status << "\n\n";

	if ( status )
	{
		std::cout << gai_strerror(status) << std::endl;
		return 0;
	}

	p = res;
	for (; p != nullptr; p = p->ai_next) {
		char ipstr[INET_ADDRSTRLEN];
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;

		// IP 주소를 문자열로 변환
		std::cout << "ai_flags: " << p->ai_flags << "\n";
		std::cout << "ai_family: " << p->ai_family << "\n";
		std::cout << "ai_socktype: " << p->ai_socktype << "\n";
		std::cout << "ai_protocol: " << p->ai_protocol << "\n";
		std::cout << "ai_addrlen: " << p->ai_addrlen << "\n";
		inet_ntop(p->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
		std::cout << "ip: " << ipstr << std::endl;
		std::cout << "ai_canonname: " << p->ai_canonname << "\n\n";
	}

	freeaddrinfo(res);

	return 0;
}

// 출처: https://kimjingyu.tistory.com/entry/호스트와-서비스-변환 [JingyuKim:티스토리]