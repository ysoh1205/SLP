#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MAXLINE 50

int main(int argc, char *argv[]){
	int n, listenfd, connfd, caddrlen;
	struct hostent *h;
	struct sockaddr_in saddr, caddr;
	char buf[60]={0, };
	char buf2[256];
	const char *copy = "_copy";
	char file1[MAXLINE] = "";
	int port = atoi(argv[1]);
	int flag=0;
	int fd1, fd2;
	size_t size;
	int filesize=0;

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		printf("socket() failed.\n");
		exit(1);
	}

	memset((char*)&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);
	if(bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr))<0){
		printf("bind() failed.\n");
		exit(2);
	}
	if(listen(listenfd, 5)<0){
		printf("listen() failed.\n");
		exit(3);
	}
	while(1){
		caddrlen = sizeof(caddr);
		if((connfd = accept(listenfd, (struct sockaddr *)&caddr, (socklen_t *)&caddrlen))<0){
			printf("accept() failed.\n");
			continue;
		}
		while((n = read(connfd, buf, MAXLINE))>0){
			filesize=0;
			if(strncmp(buf, "quit", 4)==0 && n==5){
				close(connfd);
				flag=1;
				break;
			}
			printf("got %d bytes from client.\n", n);
			write(connfd, buf, n);
			strncpy(file1, buf, n-1);
			file1[n-1]='\0';
			for(int i=0; i<5; i++){
				buf[n-1+i] = copy[i];
			}
			buf[n+4]='\0';
			printf("FILE NAME : %s\n", file1);
			fd1 = open(file1, O_RDONLY);
			fd2 = open(buf, O_RDWR | O_CREAT, 0664);
			while((size = read(fd1, buf2, 256))>0){
				write(fd2, buf2, size);
				filesize+=size;
			}
			printf("got %d bytes from client.\n", filesize);
			printf("Send %d bytes to server.\n", filesize);
		}
		
		close(connfd);
		if(flag==1){
			break;
		}
	}
	exit(0);
}
