#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAXLINE 4096
#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "./tmp/fifo.2" 
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

char data_r[MAXLINE];

int len,n,i,j;

void client(int, int), server(int, int);

int main()
{
	int readfd, writefd;
	pid_t childpid;

	if ((mkfifo(FIFO1,0666))==-1)
	{
		printf("error1\n");
		exit(1);	
	}
	if ((mkfifo(FIFO2,FILE_MODE))==-1)
	{
		unlink(FIFO1);
		printf("error2\n");
		exit(1);
	}
	printf("파일을 읽는 경우, 쓰는 경우에 따라 다음과 같이 입력해주세요\n");
        
	exit(0);
}
