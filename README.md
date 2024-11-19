# 42webserv

## request 파싱 실행 방법
study/jajo/redirection/request 안에서 make re 실행 후 실행 파일 확인
- debug.cpp 코드 지우면 request 들어온 http 메시지 디버깅 출력문 안뜹니당...!
- redirection 구현을 파트 나눠서 할 거 아니면 파싱부 부터 잡고 가야할 것 같아요
- cpp98 버전이랑 -3W 옵션 다 적용해놨습니다.

## 11/19 ksuh
# ServerBlock.cpp => setHost함수의 유효성 검사 부분에서
- 127.0.0.
- .127.0.1 
.으로 끝나거나 .으로 시작하는 것들에 대한 처리가 필요해 보입니다.

# setPort의 범위 논의
TCP/IP에서 listen() 시스템 호출을 통해 서버는 클라이언트의 연결을 수락할 준비를 하게 되며, 이를 위해 포트를 설정합니다. 포트 번호는 0부터 65535까지 총 65536개의 값으로 구성되어 있습니다. 이 중, 사용 가능한 포트의 범위는 다음과 같이 나뉩니다:

잘 알려진 포트 (Well-known ports): 0 ~ 1023

이 포트들은 특정 서비스에 예약되어 있으며, 시스템 관리자 권한이 필요한 포트입니다. 예를 들어, HTTP는 포트 80, HTTPS는 포트 443입니다.

등록된 포트 (Registered ports): 1024 ~ 49151

이 포트들은 특정 응용 프로그램에 의해 사용될 수 있지만, 시스템 관리자 권한 없이 사용할 수 있습니다. 예를 들어, MySQL은 기본적으로 포트 3306을 사용합니다.

동적/사설 포트 (Dynamic or Private ports): 49152 ~ 65535

이 포트들은 주로 클라이언트가 임시로 연결을 설정할 때 사용되며, 시스템에 의해 동적으로 할당됩니다. 보통 서버가 아닌 클라이언트 측에서 사용됩니다.

따라서 listen()을 통해 포트를 바인딩할 때 사용할 수 있는 포트 범위는 1024 ~ 65535입니다. 단, 잘 알려진 포트(0 ~ 1023)는 일반 사용자가 아닌 관리자 권한을 가진 사용자만 사용 가능합니다.

- by chatgpt

- 공식 문서에도 이에 대한 내용이 있습니다
https://learn.microsoft.com/ko-kr/troubleshoot/windows-client/networking/tcp-ip-port-exhaustion-troubleshooting

# setClientMaxBodySize 논의
(좀 극단적인 느낌이 있지만..)
1000MK - strtoul이후 M을 가리키는 상황인데 M이 문자열 끝이 아닌 경우

# setIndex, setMethods, setCgi
- 중복 검사?

# setRoot의 validateIndex
- 유효성 검사에서 +가 갖는 의미?
- 이상한 문자가 들어오는 것과 //가 연속되어 들어오는지만 검사하면 되는지?

# Conf.cpp => switchCase함수의 인자 &val

- setHost에는 val 빈 문자열 ""로 들어올 경우 아래와 같이 에러처리를 하지만
if (host.empty())
    return false;
... 다른 함수들에서는 그런 내용이 없는 것 같습니다.

# Conf parseLoc의 함수 동작원리
- parseLoc은 parseLine에서 호출이 됩니다.
- 만약에 key가 location과 일치한다면
- 탈출 조건은
- 1. location {} => location에 해당되는 value값이 없거나 경로가 이상할 때 (validatePath값이 false일때)
- 2. parseLine이 재귀로 호출되어 실행되다가 실패할 때

- 이 두 경우 모두 parseLine은 false를 반환하며 parseBlock함수로 빠져나가야 하지만
- 실제로는 그렇게 동작하지 않는 것처럼 보입니다.

# Conf line 248의 의미?
else if (*i == '{')
{
    _block.push_back(Location());
    _back = &_block.back();
    return true;
}

# Conf file
- server의 인자값으로 host, port가 필수적으로 들어갔는지에 대한 여부
- server block 내부에서 location의 인자로 host가 들어갈 수 있는지