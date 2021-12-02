#include <stdio.h>

int main(){
	char buf[256];
	fgets(buf, 256, stdin);

	printf("%s in test1\n", buf);

}
