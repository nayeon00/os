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
#define FIFO2 "/tmp/fifo.2" 
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)



int len,n,i,j;

void client(int, int), server(int, int);

int main()
{
	int readfd, writefd;
	pid_t childpid;

	printf("파일을 읽는 경우, 쓰는 경우에 따라 다음과 같이 입력해주세요\n");
        printf("*띄어쓰기 없이 입력*\n");
        printf("읽는 경우 : 파일명,r,읽을 byte 수\n");
        printf("쓰는 경우 : 파일명,w,쓸 내용\n");
	
	while(1)
	{
		
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

		childpid = fork();
		if (childpid==0){
			readfd=open(FIFO1,O_RDONLY,0);
			writefd=open(FIFO2,O_WRONLY,0);
			server(readfd, writefd);
		}
		else{
			readfd=open(FIFO2,O_RDWR,0);
			writefd=open(FIFO1,O_RDWR,0);
			client(readfd, writefd);
		}
		close(readfd);
		close(writefd);
		unlink(FIFO1);
		unlink(FIFO2);
			
	}
	
	

	return 0;
}


void client(int readfd, int writefd)
{
	char buff[MAXLINE];
	buff[0] = '\0';
	printf("\n입력해주세요 : ");

	fgets(buff, MAXLINE, stdin);
	len = strlen(buff);
	if (buff[len-1]=='\n'){
		len--;
	}
	write(writefd, buff, len);
	
	n = read(readfd, buff, MAXLINE);
	while (n>0){
		write(1, buff, n);
	}
	
	printf("\n");
	return;
}


void server(int readfd, int writefd)
{
	char buff[MAXLINE+1];
	n = read(readfd, buff, MAXLINE);
	buff[n] = '\0';
	char filename[20];
	filename[0] = '.';
	filename[1] = '/';

	for (i=0,j=2;buff[i]!=',';i++,j++){
		filename[j] = buff[i];
	}
	filename[j] = '\0';

	int fd = open(filename, O_RDONLY);
	if (fd==-1){
		printf("no file error\n");
		exit(0);
	}

	char access = buff[i+1];
        
        if (access == 'r'){ /*읽기*/
		len = strlen(buff);
		char data_r[MAXLINE]="";
		char num[5];
		for (i=i+3,j=0;i<len;i++,j++){
			num[j] = buff[i];
		}
		int byte = atoi(num);
		printf("읽기 성공!\n");
		n = read(fd, data_r, byte);
		data_r[n]='\0';
		puts(data_r);
		close(fd);
	}
	else if (access == 'w'){ /*쓰기*/
		close(fd);
		fd = open(filename, O_WRONLY | O_APPEND);
		if (fd==-1){
			printf("no file error\n");
			exit(0);
		}
		else{
			char data_w[MAXLINE]="";
			for (i=i+3,j=0;buff[i]!='\0';i++,j++){
				data_w[j] = buff[i];
			}
			printf("쓰기 성공!\n");
			write(fd, data_w, strlen(data_w));
			int byte = strlen(data_w);
			char num[5];
			sprintf(num, "%d", byte);
			puts(num);
			close(fd);
		}
	}
	else{
                printf("error\n");
                exit(0);
        }
	
	return;	
}
