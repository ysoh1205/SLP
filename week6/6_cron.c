#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	unsigned int pid;
	time_t t;
	struct tm *tm;	
	FILE* fd;
	char *argv[3];
	char buf[512];
	int fd0, fd1, fd2;
	size_t size;
	int exec_min, exec_hour;
	int min, hour;
	int flag = 1;
	
	fd = fopen("./crontab", "r");
	pid = fork();
	
	if(pid == -1) return -1;
	if(pid != 0)
		exit(0);
	if(setsid() < 0)
		exit(0);
	if(chdir("/") < 0)
		exit(0);

	umask(0);

	close(0);
	close(1);
	close(2);

	fd0 = open("/dev/null", O_RDWR);
	fd1 = open("/dev/null", O_RDWR);
	fd2 = open("/dev/null", O_RDWR);

	t = time(NULL);
	tm = localtime(&t);	
	exec_min = tm->tm_min;
	exec_hour = tm->tm_hour;
	
	while (1)
	{
		t = time(NULL);
		tm = localtime(&t);
		if(pid==0){
			buf[0] = '\0';	
			fgets(buf, 512, fd);
			size = strlen(buf);
			char *pos = buf;
			int i=0;
			while((argv[i++] = strtok_r(pos, " ", &pos))){}

			if(strcmp(argv[0],"*")==0) min=60;
			else min = atoi(argv[0]);

			if(strcmp(argv[1],"*")==0) hour=24;
			else hour = atoi(argv[1]);
		}
		pid = fork();
		
		// insert your code
		

		// ##  hints  ##	
		if(pid!=0){			
			if(min==60 && hour==24){
				execl("/bin/sh", "/bin/sh", "-c", argv[2], (char*) NULL);
			}else if(min!=60 && hour==24){
				if(tm->tm_min==min) execl("/bin/sh", "/bin/sh", "-c", argv[2], (char*) NULL);
			}
			else if(min==60 && hour!=24){
				if(tm->tm_hour==hour) execl("/bin/sh", "/bin/sh", "-c", argv[2], (char*) NULL);
			}
			else{
				if(tm->tm_hour==hour && tm->tm_min==min) execl("/bin/sh", "/bin/sh", "-c", argv[2], (char*) NULL);
			}
		}
		sleep(60 - tm->tm_sec % 60);
	}

	return 0;
}
