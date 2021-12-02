#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
	int child_status;

	if(fork()==0){
		puts("child");
	}else{
		puts("parent");
		wait(&child_status);
//		puts("parent");
	}
}
