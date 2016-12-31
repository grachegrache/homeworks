#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_BYTE 128
#define BUFF_SIZE 1024

int main(int argc, char* argv[]){
    //정해진 인자가 입력이 되지 않으면 종료
    if(argc < 3){
        printf("#[실행파일 명] [경로/디바이스명] [전송 횟수]\n");
        return -1;
    }
    
    //변수 정의
    int fd, cnt_write;
    char str[BUFF_SIZE];
    memset(str, 0, BUFF_SIZE);
    
    //파라미터 가져오기
    if((fd = open(argv[1], O_RDWR)) < 0){
        printf("[경로/디바이스명] 오류\n");
        return -1;
    }
    cnt_write = atoi(argv[2]);
    
    //전송 횟수만큼 write
    while(cnt_write > 0)
        if(scanf("%s", str) > 0){
            write(fd, str, strlen(str));
            cnt_write--;
        }
    
    //read
    if(read(fd, str, BUFF_SIZE) <= 0)
    	printf("read error\n");
    printf("%s\n", str);
    
    close(fd);
    
    return 0;
}
