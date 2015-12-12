#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 80 /* The maximum length command */ 

int parser(char *buffer , char **tokens){
	int ret = 0;
	//printf("DEBUGGER0\n");
	if(!buffer){
		//printf("DEBUGGER1\n");
		return ret;
	}
	if(strlen(buffer) == 0){
		//printf("DEBUGGER2\n");
		return ret;
	}
	char *tmp = strtok(buffer, " ");
	*tokens = tmp;
	//printf("tokens[%d] is : %s\n", ret, *tokens);
	ret++;
	while(tmp){
		tmp = strtok(NULL, " ");
		*(tokens+ret) = tmp;
		//printf("tokens[%d] is : %s\n", ret, tmp);
		if(tmp){
			ret++;
		}
	}
	return ret;
}

int chmop(char *buffer){
	if(!buffer){
		//printf("DEBUGGER1\n");
		return 0;
	}
	if(strlen(buffer) == 0){
		//printf("DEBUGGER2\n");
		return 0;
	}
	if(*(buffer+strlen(buffer)-1) == '\n'){
		*(buffer+strlen(buffer)-1) = '\0';
	}
	return 1;
}

int main(void){
	char *args[MAXLINE/2 + 1]; /* command line with max 40 arguments */ 
	int should_run = 1; /* flag to determine when to exit program */ 
	printf("CS149 Shell from Jingyuan Chen\n"); /* replace w/ name */ 
	while (should_run) {
		printf("Jingyuan-536>"); /* prompt- replace FirstName and L3SID */
		char* buffer = (char*)malloc(sizeof(char)*MAXLINE);
		fgets(buffer, MAXLINE, stdin);
		chmop(buffer);
		if(!strcmp(buffer,"exit")){
			should_run = 0;
			continue;
		}
  		fflush(stdout);

  		int pid = fork();

  		//if we are in child process
  		char **tokens = (char**)malloc(sizeof(char *)*MAXLINE/2);	

  		int argv = parser(buffer,tokens);
  		int should_wait = 1;
  		int childPid = 0;
  		if(! strcmp(*(tokens+argv-1) ,"&")){
			should_wait = 0;
  		}
  		if(pid<0){
  			printf("ERROR: CANNOT FORK PROPERLY\n");
  			continue;
  		}
  		else if(!pid){
  			//parser this buffer to a char* array
  			childPid = getpid();
  			if(! argv ){
  				exit(0);
  			}
  			else{
  				//run the command line
  				int i;
  				if( should_wait){
  					*(tokens+argv) = NULL;
  				}
  				else{
  					*(tokens+argv-1) = NULL;
  				}
  				int rel = execvp(*tokens,tokens);
  				if(rel < 0){
  					printf("ERROR: ILLEGAL SHELL COMMAND\n");
  				}
  				exit(0);
  			}
  		}
  		else{
  			if(should_wait){
  				wait(&childPid);

  			}
  		}

		/* After reading user input, the steps are:
 		* (1) fork a child process using fork()
 		* (2) the child process will invoke execvp()
 		* (3) if command included &, parent will NOT invoke wait()*/ 
 	}
	return 0; 
}

