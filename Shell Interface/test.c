#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int main(){
	int a = 0;
	int pid = fork();
	if(pid == 0){
		a= 2;
		printf("child: a = %d\n",a);
	}
	else{
		printf("parent: a = %d\n",a);
	}
	return 0;
}
