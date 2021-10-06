#include <stdio.h>
#include <string.h>
#include <unistd.h>
int main(){
	FILE* fd = fopen("./crontab", "r");	
	char *argv[3];

	

//	char str[512] = "System Software Labatory";
	char str[512];
	char *pos = str;
	int i=0;
	
	fork();
	fgets(str, 512, fd);
	printf("%s, size is %d\n", str, strlen(str));
//	printf("%s\n", argv[1]);
//	printf("%s\n", argv[2]);
}
