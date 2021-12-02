//오윤석_2020312416

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define TRUE 1
#define FALSE 0

char *cmd_type1[] = {"ls", "man", "grep", "sort", "awk", "bc"};
char *cmd_type2[] = {"head", "tail", "cat", "cp"};
char *cmd_type3[] = {"mv", "rm", "cd"};
char *cmd_type4[] = {"pwd", "exit"};

int separateCmd(char* cmd, char* cmd1[]){
	int i=0;
	char *ptr;
	ptr = strtok(cmd, "|");
	while(ptr!=NULL){
		if(ptr[0] == ' ') cmd1[i++] = ptr+1;
		else cmd1[i++] = ptr;
		ptr = strtok(NULL , "|");
	}

	return i;
}

void make_tokens(char* cmd, char* arg[], char* arg2[], char* target){
	int i=0;
	char* ptr;
	ptr = strtok(cmd, " ");
	while(strcmp(ptr, target)!=0){
		arg[i++] = ptr;
		ptr = strtok(NULL, " ");
	}
	ptr = strtok(NULL, " ");
	arg[i]=NULL;
	i=0;
	while(ptr!=NULL){
		arg2[i++] = ptr;
		ptr = strtok(NULL, " ");
	}
	arg2[i] = NULL;
	return;
}

void make_tokens2(char* cmd, char* arg[], char* arg2[], char* arg3[], char* target1, char* target2){
	int i=0;
	char* ptr;
	ptr = strtok(cmd, " ");
	while(strcmp(ptr, target1)!=0){
		arg[i++] = ptr;
		ptr = strtok(NULL, " ");
	}
	ptr = strtok(NULL, " ");
	arg[i] = NULL;
	i=0;
	while(strcmp(ptr, target2)!=0){
		arg2[i++] = ptr;
		ptr = strtok(NULL, " ");
	}
	ptr = strtok(NULL, " ");
	arg2[i]=NULL;
	i=0;
	while(ptr!=NULL){
		arg3[i++] = ptr;
		ptr = strtok(NULL, " ");
	}
	arg3[i]=NULL;
	return;
}

int whatCmdType(char arg[]){
	for(int i=0; i<6; i++){
		if(strcmp(arg, cmd_type1[i])==0) return 1;
	}
	if(arg[0]=='.' && arg[1]=='/') return 1;
	for(int i=0; i<4; i++){
		if(strcmp(arg, cmd_type2[i])==0) return 2;
	}
	for(int i=0; i<3; i++){
		if(strcmp(arg, cmd_type3[i])==0) return 3;
	}
	for(int i=0; i<2; i++){
		if(strcmp(arg, cmd_type4[i])==0) return 4;
	}
	return 0;
}

int isPath(char arg[]){
	if(arg[0]=='.' && arg[1]=='/') return TRUE;
	else return FALSE;
}

void handler(int sig){
	return;
}

int main(){
	size_t size;
	char *cmd;
	char *cmd1[40];
	char *ptr;
	char *arg[128];
	char *arg2[128];
	char *arg3[128];
	char path[512];
	char dir[256];
	char buf[512];
	int fd[2];
	int tempFd;
	int fdr;
	int fdr2;
	int child_status;
	
	signal(SIGINT, handler);
	while(1){

		int num=0;
		int cmdCnt;
		cmd = NULL;
		getline(&cmd, &size, stdin);
		cmd[strlen(cmd)-1] = '\0';
		if(strlen(cmd)==0) continue;
		cmdCnt = separateCmd(cmd, cmd1);

		if(cmdCnt==1){
			if(strchr(cmd1[0],'<')!=NULL && strchr(cmd1[0],'>')!=NULL){

				make_tokens2(cmd1[0], arg, arg2, arg3, "<", ">");
				sprintf(path, "/bin/%s", arg[0]);

				fdr = open(arg2[0], O_RDONLY);
				fdr2 = open(arg3[0], O_RDWR | O_CREAT, 0664);
				if(pipe(fd)<0) exit(1);

				pid_t pid;
				pid = fork();
				
				if(pid<0) exit(2);
				if(pid==0){
					if((pid=fork())<0) exit(2);
					if(pid==0){
						pid_t pid2 = fork();
						close(fd[0]);
						dup2(fd[1],1);
						if(pid2==0){
							dup2(fdr,0);
							execv(path, arg);
						}else{
							wait(&child_status);
						}

					} else{
						close(fd[1]);
						size = read(fd[0], path, 512);
						write(fdr2, path, size);
						exit(0);
					}
					exit(0);
				}
			
			} else if(strchr(cmd1[0], '<')!=NULL){

				make_tokens(cmd1[0], arg, arg2, "<");
				sprintf(path, "/bin/%s", arg[0]);

				fdr = open(arg2[0], O_RDONLY);
				
				pid_t pid;
				pid = fork();

				if(pid<0) exit(2);
				if(pid==0){
					dup2(fdr,0);
					execv(path,arg);
				}else{
					wait(&child_status);
				}
					
			} else if(strstr(cmd1[0], ">>")!=NULL){
				
				make_tokens(cmd1[0], arg, arg2, ">>");
				sprintf(path, "/bin/%s", arg[0]);

				fdr = open(arg2[0], O_RDWR | O_CREAT, 0664);
				lseek(fdr, 0, SEEK_END);
				if(pipe(fd)<0) exit(1);

				pid_t pid;
				pid = fork();

				if(pid<0) exit(2);
				if(pid==0){
					if((pid=fork())<0) exit(2);
					if(pid==0){
						close(fd[1]);
						size = read(fd[0], path, 512);
						write(fdr, path, size);
						exit(0);
					}else{
						close(fd[0]);
						dup2(fd[1], 1);
						close(fd[1]);
						execv(path, arg);
						wait(&child_status);
					}
				}

			} else if(strchr(cmd1[0], '>')!=NULL){
				
				make_tokens(cmd1[0], arg, arg2, ">");
				sprintf(path, "/bin/%s", arg[0]);

				fdr = open(arg2[0], O_RDWR | O_CREAT, 0664);
				if(pipe(fd)<0) exit(1);

				pid_t pid;
				pid = fork();

				if(pid<0) exit(2);
				if(pid==0){
					if((pid=fork())<0) exit(2);
					if(pid==0){
						close(fd[1]);
						size = read(fd[0], path, 512);
						write(fdr, path, size);
						exit(0);
					}else{
						close(fd[0]);
						dup2(fd[1], 1);
						close(fd[1]);
						execv(path, arg);
						wait(&child_status);
					}
				}

			} else{
				ptr = strtok(cmd1[0], " ");
				while(ptr!=NULL){
					arg[num++] = ptr;
					ptr = strtok(NULL, " ");
				}
				arg[num] = NULL;
				if(isPath(arg[0])){
					getcwd(dir, 256);
					strcpy(path, dir);
					strcpy(dir, arg[0]+2);
					sprintf(buf, "%s/%s", path, dir);
					sprintf(path, "%s", buf);
				} else{
					sprintf(path, "/bin/%s", arg[0]);
				}
				
				if(strcmp(arg[0], "exit")==0){
					exit(0);
				}
				if(strcmp(arg[0], "cd")==0){
					chdir(arg[1]);	
				}

				if(fork()==0){				
					execv(path, arg);
					exit(0);
				} else{
					wait(&child_status);
				}
			}
		}
		else{
			
			pid_t pid0;
			pid0 = fork();

			if(pid0<0) exit(2);
			if(pid0==0){
				if(pipe(fd)<0) exit(1);

				dup2(1, tempFd);

				if(strchr(cmd1[0], '<')!=NULL){

					make_tokens(cmd1[0], arg, arg2, "<");
					sprintf(path, "/bin/%s", arg[0]);

					fdr = open(arg2[0], O_RDONLY);
					
					pid_t pid;
					pid = fork();

					if(pid<0) exit(2);
					if(pid==0){
						dup2(fdr,0);
						dup2(fd[1], 1);
						close(fd[1]);
						close(fd[0]);
						execv(path,arg);
					}else{
						wait(&child_status);
					}
						
				} else{
					ptr = strtok(cmd1[0], " ");
					while(ptr!=NULL){
						arg[num++] = ptr;
						ptr = strtok(NULL, " ");
					}
					arg[num] = NULL;
					if(isPath(arg[0])){
						getcwd(dir, 256);
						strcpy(path, dir);
						strcpy(dir, arg[0]+2);
						sprintf(buf, "%s/%s", path, dir);
						sprintf(path, "%s", buf);
					} else{
						sprintf(path, "/bin/%s", arg[0]);
					}
					
					if(strcmp(arg[0], "exit")==0){
						exit(0);
					}
					if(strcmp(arg[0], "cd")==0){
						chdir(arg[1]);	
					}

					if(fork()==0){
						dup2(fd[1], 1);
						close(fd[1]);
						close(fd[0]);		
						execv(path, arg);

					} else{
						wait(&child_status);
					}
				}
				num=0;
				for(int i=1; i<cmdCnt-1; i++){
					num=0;
					ptr = strtok(cmd1[i], " ");
					while(ptr!=NULL){
						arg[num++] = ptr;
						ptr = strtok(NULL, " ");
					}
					arg[num] = NULL;
					if(isPath(arg[0])){
						getcwd(dir, 256);
						strcpy(path, dir);
						strcpy(dir, arg[0]+2);
						sprintf(buf, "%s/%s", path, dir);
						sprintf(path, "%s", buf);
					} else{
						sprintf(path, "/bin/%s", arg[0]);
					}
					
					if(strcmp(arg[0], "exit")==0){
						exit(0);
					}
					if(strcmp(arg[0], "cd")==0){
						chdir(arg[1]);	
					}
					if(fork()==0){		
						dup2(fd[0],0);
						dup2(fd[1],1);
						close(fd[0]);
						close(fd[1]);
						execv(path, arg);
					} else{
						wait(&child_status);
					}
				}
				dup2(tempFd, 1);
				close(tempFd);
				num=0;
				
				if(strchr(cmd1[cmdCnt-1],'>')!=NULL && strstr(cmd1[cmdCnt-1], ">>")==NULL){
					
					make_tokens(cmd1[cmdCnt-1], arg, arg2, ">");
					sprintf(path, "/bin/%s", arg[0]);

					fdr = open(arg2[0], O_RDWR | O_CREAT, 0664);
					if(isPath(arg[0])){
						getcwd(dir, 256);
						strcpy(path, dir);
						strcpy(dir, arg[0]+2);
						sprintf(buf, "%s/%s", path, dir);
						sprintf(path, "%s", buf);
					}
					pid_t pid;
					pid = fork();

					if(pid<0) exit(2);
					if(pid==0){
						dup2(fd[0],0);
						dup2(fd[1],1);
						execv(path,arg);
						exit(0);
					}else{
						wait(&child_status);
						size = read(fd[0], path, 512);
						write(fdr, path, size);

					}


					
				} else if(strstr(cmd1[cmdCnt-1], ">>")!=NULL){

					make_tokens(cmd1[cmdCnt-1], arg, arg2, ">>");
					sprintf(path, "/bin/%s", arg[0]);

					fdr = open(arg2[0], O_RDWR | O_CREAT, 0664);
					lseek(fdr, 0, SEEK_END);
					if(isPath(arg[0])){
						getcwd(dir, 256);
						strcpy(path, dir);
						strcpy(dir, arg[0]+2);
						sprintf(buf, "%s/%s", path, dir);
						sprintf(path, "%s", buf);
					}

					pid_t pid;
					pid = fork();

					if(pid<0) exit(2);
					if(pid==0){
						dup2(fd[0],0);
						dup2(fd[1],1);
						execv(path,arg);
						exit(0);
					}else{
						wait(&child_status);
						size = read(fd[0], path, 512);
						write(fdr, path, size);
					}
					
				} else{
					ptr = strtok(cmd1[cmdCnt-1], " ");
					while(ptr!=NULL){
						arg[num++] = ptr;
						ptr = strtok(NULL, " ");
					}
					arg[num] = NULL;
					if(isPath(arg[0])){
						getcwd(dir, 256);
						strcpy(path, dir);
						strcpy(dir, arg[0]+2);
						sprintf(buf, "%s/%s", path, dir);
						sprintf(path, "%s", buf);
					} else{
						sprintf(path, "/bin/%s", arg[0]);
					}
					
					if(strcmp(arg[0], "exit")==0){
						exit(0);
					}
					if(strcmp(arg[0], "cd")==0){
						chdir(arg[1]);	
					}

					if(fork()==0){
						dup2(fd[0], 0);
						execv(path, arg);
						exit(0);
					} else{
						wait(&child_status);
					}
				}
				exit(0);
			}else{
				wait(&child_status);
			}
		}
	}

}
