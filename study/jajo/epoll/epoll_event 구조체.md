>이 구조체는 `epoll`이 감시할 이벤트와 관련된 정보를 저장합니다.

```cpp
struct epoll_event {
    uint32_t events;   // 감시할 이벤트
    union {
        void *ptr;     // 사용자 정의 포인터
        int fd;       // 파일 디스크립터
        uint32_t u32; // 32비트 정수
        uint64_t u64; // 64비트 정수
    } data;           // 사용자 정의 데이터
};
```

