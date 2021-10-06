#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
int main(){
	unsigned int pid;
	int fd0, fd1, fd2;

	if((pid = fork()) != 0) exit(0);
	if(setsid() < 0) exit(0);
	if(chdir("/") < 0) exit(0);

	umask(0);

//	close(0); close(1); close(2);

/*	fd0 = open("/dev/null", O_RDWR);
	fd1 = open("/dev/null", O_RDWR);
	fd2 = open("/dev/null", O_RDWR);
*/
	printf("hi\n");
	while(1){
//		pid = fork();
		printf("daemon\n");
		sleep(2);
	}
	return 0;
}
