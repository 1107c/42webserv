#include <stdio.h>
#include <sys/select.h>

int main() {
    printf("FD_SETSIZE = %d\n", FD_SETSIZE);
    
    // fd_set의 실제 크기 확인
    printf("sizeof(fd_set) = %lu bytes\n", sizeof(fd_set));
    
    // 비트 단위로 계산
    printf("sizeof(fd_set) in bits = %lu bits\n", sizeof(fd_set) * 8);
    
    return 0;
}