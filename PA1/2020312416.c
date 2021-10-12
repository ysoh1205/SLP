#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

int itoa(int n, char* buf){
	int temp = n;
	int digit = 0;
	int ret;
	if(n==0){
		digit++;
		buf[0] = 48;
		return digit;
	}
	while(temp!=0){
		temp/=10;
		digit++;
	}
	ret = digit;

	while(n!=0){
		digit--;
		buf[digit] = n%10 + 48;
		n/=10;
	}
	return ret;
}
int notChar(char c){
	return (c>=65 && c<=90)||(c>=97 && c<=122) ? 1:0;
}
int sameAlphabet(char c1, char c2){
	if(!(c1>=65 && c1<=122) && !(c1>=48 && c1<=57)) return FALSE;
	return (c2-c1)%32==0 ? TRUE:FALSE;
}

int setFlag(char *str, int len){
	if(str[0]=='"'&& str[len-1]=='"') return 3;
	for(int i=0; i<len; i++){
		if(str[i]=='*') return 4;
		if(str[i]==' ') return 2;
	}
	return 1;
}

void containWord1(char* buf, char* input, int size, int len, int lineNum){
	int flag = FALSE;
	char output[20];
	for(int i=0; i<=size; i++){
		for(int j=0; j<len; j++){
			flag=TRUE;
			if(j==0 && i!=0 && buf[i-1]!=' '){
				flag = FALSE;
				break;
			}
			if(sameAlphabet(buf[i],input[j])){
				i++;
			}else{
				flag = FALSE;
				break;
			}
			if(j==len-1 && buf[i]!=' ' && buf[i]!='\n') flag = FALSE;
		}
		
		//Save linenum:colnum
		if(flag){
			int a1 = itoa(lineNum+1, output);
			output[a1]=':';
			int a2 = itoa(i-len, output+a1+1);
			output[a1+a2+1]=' ';
			write(1, output, a1+a2+2);
		}

	}
}

void containWord2(char* buf, char* input, int size, int len, int lineNum){
	int flag=FALSE;
	char output[20];
	int start=0, end;
	int wordNum=1;
	//Count number of words
	for(int i=0; i<=len; i++){
		if(input[i]==' ') wordNum++;
	}
	for(int i=0; i<wordNum; i++){
		//Save start and end point of a word
		for(int j=start; j<len; j++){
			if(input[j+1]==' ' || j+1==len){
				end = j+1;
				break;
			}
		}

		for(int j=0; j<=size; j++){
			flag = FALSE;
			if(sameAlphabet(buf[j], input[start])){

				for(int k=start; k<end; k++){
					flag = TRUE;
					if(j!=0 && k==start && buf[j-1]!=' '){
						flag=FALSE;
						break;
					}
					if(sameAlphabet(buf[j], input[k])){
						j++;
					}else{
						flag = FALSE;
						break;
					}
					if(k==end-1 && buf[j]!=' ' && buf[j]!='\n') flag = FALSE;
				}
			}
			if(flag) break;
		}
		start = end+1;
		if(flag==FALSE) break;
	}
	if(flag){
		int a1 = itoa(lineNum+1, output);
		output[a1]=' ';
		write(1, output, a1+1);
	}

}

void containWord3(char* buf, char* input, int size, int len, int lineNum){
	int flag=FALSE;
	char output[20];
	int start1=1, end1, start2, end2;
	int END;
	for(int i=start1; i<len; i++){
		if(input[i]==' '){
			end1 = i;
			break;
		}
	}
	start2 = end1+1;
	end2 = len-1;
	for(int i=0; i<=size; i++){
		if(sameAlphabet(buf[i], input[start1])){
			for(int j=start1; j<end1; j++){
				flag = TRUE;
				if(i!=0 && j==start1 && buf[i-1]!=' '){
					flag=FALSE;
					break;
				}
				if(sameAlphabet(buf[i], input[j])){
					i++;
				}else{
					flag=FALSE;
					break;
				}
//				if(j==end1-1 && buf[j]!=' ' && buf[j]!='\n') flag=FALSE;
			}
		}else{
			continue;
		}
		if(buf[i]!=' ' || flag==FALSE){
			flag=FALSE;
			continue;
		}
		while(buf[i]==' '){
			i++;
		}
		
		if(sameAlphabet(buf[i], input[start2])){
			for(int j=start2; j<end2; j++){
				flag = TRUE;
				if(sameAlphabet(buf[i], input[j])){
					i++;
				}else{
					flag=FALSE;
					break;
				}
//				if(j==end2-1 && buf[j]!=' ' && buf[j]!='\n') flag = FALSE;
			}
		}else{
			flag=FALSE;
			continue;
		}
		
		if(flag==FALSE) continue;
		if(flag==TRUE){
			END=i;
			break;
		}
	}

	if(flag){
		int a1 = itoa(lineNum+1, output);
		output[a1]=':';
		int a2 = itoa(END-len+2, output+a1+1);
		output[a1+a2+1]=' ';
		write(1, output, a1+a2+2);
	}

}

void containWord4(char* buf, char* input, int size, int len, int lineNum){
	int flag=FALSE;
	char output[20];
	int start1=0, end1, start2, end2;
	int END;
	for(int i=start1; i<len; i++){
		if(input[i]=='*'){
			end1 = i;
			break;
		}
	}
	start2 = end1+1;
	end2 = len;
	for(int i=0; i<=size; i++){
		if(sameAlphabet(buf[i], input[start1])){
			for(int j=start1; j<end1; j++){
				flag = TRUE;
				if(j==start1 && i!=0 && buf[i-1]!=' '){
					flag = FALSE;
					break;
				}
				if(i!=0 && j==start1 && buf[i-1]!=' '){
					flag=FALSE;
					break;
				}
				if(sameAlphabet(buf[i], input[j])){
					i++;
				}else{
					flag=FALSE;
					break;
				}
				if(j==end1-1 && buf[i]!=' ' && buf[i]!='\n') flag=FALSE;
			}
		}
		if(flag==TRUE){
			END = i;
			break;
		}
	}

	if(flag==FALSE) return ;
	
	while(buf[END]==' ') {
		END++;
	}
	while(buf[END]!=' ' && END<size) {
		END++;
	}
	for(int i=END; i<=size; i++){
		flag = FALSE;
		if(sameAlphabet(buf[i], input[start2])){
			for(int j=start2; j<end2; j++){
				flag = TRUE;
				if(j==start2 && i!=0 && buf[i-1]!=' '){
					flag = FALSE;
					break;
				}
				if(sameAlphabet(buf[i], input[j])){
					i++;
				}else{
					flag=FALSE;
					break;
				}
				if(j==end2-1 && buf[i]!=' ' && buf[i]!='\n') flag = FALSE;
			}
		}
		if(flag==TRUE) break;
	}

	if(flag){
		int a1 = itoa(lineNum+1, output);
		output[a1]=' ';
		write(1, output, a1+1);
	}

}

int main(int argc, char** argv){
	int* lineSize = calloc(10000, sizeof(int));
	int fd = open(argv[1], O_RDWR);
	int lineNum=0;
	int filesize = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	char test;
	char newLine[] = "\n";

	for(int i=0; i<filesize; i++){
		read(fd, &test, 1);
		lineSize[lineNum]++;
		if(test=='\n'){
			lineNum++;
			continue;
		}
	}
	lseek(fd, 0, SEEK_SET);
	int max = 0;
	for(int i=0; i<lineNum; i++){
		if(max < lineSize[i]) max = lineSize[i];
	}

//Setting buffer
	char *buf = malloc(sizeof(char)*max);
	char *input = malloc(sizeof(char)*max);
	char *output;
	int flag;
//Getting Inputs
	while(1){
		lseek(fd, 0, SEEK_SET);
		read(0, input, max);
		int len;
		for(int i=0; i<max; i++){
			if(input[i]=='\n'){
				len = i;
				break;
			}
		}
		flag = setFlag(input, len);

		switch(flag){
			case 1:
				for(int i=0; i<lineNum; i++){
					read(fd, buf, lineSize[i]);
					containWord1(buf, input, lineSize[i], len, i);
				}
				write(1, newLine, 1);
				break;
			case 2:
				for(int i=0; i<lineNum; i++){
					read(fd, buf, lineSize[i]);
					containWord2(buf, input, lineSize[i], len, i);
				}
				write(1, newLine, 1);
				break;
			case 3:
				for(int i=0; i<lineNum; i++){
					read(fd, buf, lineSize[i]);
					containWord3(buf, input, lineSize[i], len, i);
				}
				write(1, newLine, 1);
				break;
			case 4:
				for(int i=0; i<lineNum; i++){
					read(fd, buf, lineSize[i]);
					containWord4(buf, input, lineSize[i], len, i);
				}
				write(1, newLine, 1);
				break;
		}

	}
	

}
