#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
	while(1){
		char *cmd;
		size_t size;
		getline(&cmd, &size, stdin);
		cmd[strlen(cmd)-1] = '\0';

		int i=0;
		char *arg[10];
		char *ptr = strtok(cmd, " ");
		while(ptr!=NULL){
			arg[i++] = ptr;
			ptr = strtok(NULL, " ");
		}
		arg[i] = NULL;
		if(strcmp("quit", arg[0])==0){
			exit(0);
		}
		char path[100];
		sprintf(path, "/bin/%s", arg[0]);
	
		pid_t pid=fork();
		if(pid==0) {
			execv(path, arg);
		}
		wait(NULL);
	}

}
