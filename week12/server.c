#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXUSER 10
#define MAXNAME 10
#define MAXLINE 80

int main (int argc, char *argv[]) {

    int n, listenfd, connfd, caddrlen, fdmax, fdnum, usernum;
    struct hostent *h;
    struct sockaddr_in saddr, caddr;
    char buf[MAXLINE];
    char name[MAXUSER][MAXNAME];
    char temp[MAXLINE+MAXNAME+1];
    int port = atoi(argv[1]);
	
    fd_set readset, copyset;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(1);
    }

    memset((char *)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        printf("bind() failed.\n");
        exit(2);
    }

    if (listen(listenfd, 5) < 0) {
        printf("listen() failed.\n");
        exit(3);
    }

    /* Insert your code for file descriptor set
	*
	*
	**/
	FD_ZERO(&readset);
	FD_SET(listenfd, &readset);
	fdmax = listenfd;

    while (1) {

        copyset = readset;

        if((fdnum = select(fdmax+1, &copyset, NULL, NULL, NULL))  < 0 ) {
            printf("select() failed.\n");
            exit(4);
        }

       for (int i = 3; i < fdmax + 1; i++) {

           memset(buf, 0, MAXLINE);
           memset(temp, 0, MAXLINE+MAXNAME+1);

            if (FD_ISSET(i, &copyset)) {

                if (i == listenfd) {
                    
                    caddrlen = sizeof(caddr);
                    if ((connfd = accept(listenfd, (struct sockaddr *)&caddr, (socklen_t *)&caddrlen)) < 0) {
                        printf ("accept() failed.\n");
                        continue;
                    }

                    /* Insert your code */ 
					FD_SET(connfd, &readset);
					if(fdmax<connfd) fdmax = connfd;
					n = read(connfd, buf, MAXNAME);
					strcpy(name[connfd-3], buf);

                }
                else {
					
                    /* Insert your code */
                    if((n = read(i, buf, MAXLINE))>0){
						printf("got %d bytes from user %s.\n", n, name[i-3]);
						sprintf(temp, "%s:%s",name[i], buf);
						for(int j=4; j<fdmax-3; j++){
							write(j, temp, MAXLINE+MAXNAME+1);
						}
					}else{
						FD_CLR(i, &readset);
						printf("connection terminated.\n");
						close(i);
					}
                }
            }  
        }
    }

    return 0;

}

