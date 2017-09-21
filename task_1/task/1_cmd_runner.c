/*
  Write a program which allow you to: 
- run another programs via command line.
- get exit codes of terminated programs

## TIPS:
1. Use "2_fork_wait_exit.c" and "4_exec_dir.c" from examples. Combine them.
2. Parse input string according to the type of exec* (see "man exec").
   a) if execvp is used, string splitting into "path" and "args" is all you need.
3. Collect exit codes via waitpid/WEXITSTATUS.
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>

int main(){
  char* argv[100];
  char *name;
  char *token,**ptr;
  char str[101];
  char s[2]=" ";
  
  int error=0;
  int status;
  pid_t pid;

  printf("$ ");
  gets(&str);
  while(strcmp(str,"exit")!=0){
      ptr=argv;
      
      token=strtok(str,s);
      
      while(token!=NULL){
      *ptr=token;
      ptr++;
      token=strtok(NULL,s);
      }
      *ptr=token;
   
      pid=fork();
      if(pid==0){
      	error=execvp(*argv,argv);
      	if(error!=0){
        	printf("Error! No such command.\n");
        	exit(-1);
      }
    }

      else if(pid==-1){
	puts("Error! Unable to create child process.");
	exit(1);
      }
      else{
	wait(&status);
	printf("Ret code: %d\n", WEXITSTATUS(status));
	printf("$ ");
      }
      gets(&str);

  }

  return 0;
}
