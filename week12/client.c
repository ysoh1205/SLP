#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 10
#define MAXLINE 80

int main (int argc, char *argv[]) {

    int n, cfd, fdmax, fdnum;
    struct hostent *h;
    struct sockaddr_in saddr;
    char buf[MAXLINE];
    char name[MAXNAME];
    char temp[MAXLINE+MAXNAME+1];
    char *host = argv[1];
    int port = atoi(argv[2]);
    fd_set readset, copyset;

//    printf("Insert your name : ");
//    scanf("%s", name); 

    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(1);
    }

    if ((h = gethostbyname(host)) == NULL) {
        printf("invalid hostname %s\n", host);
        exit(2);
    } 

    memset((char *)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    memcpy((char *)&saddr.sin_addr.s_addr, (char *)h->h_addr, h->h_length);
    saddr.sin_port = htons(port);

    if (connect(cfd,(struct sockaddr *)&saddr,sizeof(saddr)) < 0) {
        printf("connect() failed.\n");
        exit(3);
    }

    /* Insert your code for file descriptor set
	*
	*
	**/
//	write(cfd, name, MAXNAME);
	FD_ZERO(&readset);
	FD_SET(cfd, &readset);
	fdmax = cfd;

	while((n=read(0, buf, MAXLINE))>0){
		if(strncmp("quit", buf, 4)==0 && n==5){
			close(cfd);
			exit(0);
		}
		write(cfd, buf, n);
		memset(buf, 0, MAXLINE);
	}

    while (1) {

        memset(buf, 0, MAXLINE);
 	    memset(temp, 0, MAXLINE+MAXNAME+1);

       	copyset = readset;

        if ((fdnum = select(fdmax+1, &copyset, NULL, NULL, NULL)) < 0) {
   	        printf("select() failed.\n");
       	    exit(4);
        }
		
       	/* Insert your code */
		
		

		for(int i=0; i<fdmax+1; i++){
			if(FD_ISSET(i, &copyset)){
				if(i==cfd){
					n = read(cfd, temp, MAXLINE+MAXNAME+1);
					printf("%s",temp);
				}	
			}
		}
   	}
}

