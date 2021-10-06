#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int itoa(int n, char buf[]){
	int temp = n;
	int digit = 0;

	while(temp!=0){
		temp/=10;
		digit++;
	}

	buf[digit] = ' ';
	buf[digit+1] = '|';
	buf[digit+2] = ' ';
	int ret = digit+2;

	while(n!=0){
		digit--;
		buf[digit] = n%10 + 48;
		n/=10;
	}
	
	return ret;
}

int main(int argc, char** argv){
	char buf[100];
	char index[10];
	char filename[] ="Aladdin_num.txt";
	char c;
	int fd1 = open(argv[1], O_RDONLY);
	int fd2 = open(filename, O_RDWR | O_CREAT, 0755);
	int filesize = lseek(fd1, 0, SEEK_END);
	lseek(fd1, 0, SEEK_SET);
	int size, cnt = 0;
	off_t pos;
	
	do{
		cnt++;
		size = itoa(cnt, index);
	
		for(int i=0; i<100; i++){
			read(fd1, buf+i,1);
			c=buf[i];
			pos=i;
			if(c=='\0' || c=='\n'){
			       	break;
			}
		}
		write(fd2, index, size+1);
		write(fd2, buf, pos+1);
		if(lseek(fd1, 0, SEEK_CUR)>=filesize) break;
		
	}while(c!=0);
	return 0;
}
