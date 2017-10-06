/*
Write a duplex pipe implementation.
typedef struct {
  int txd[2]; 
  int rxd[2]; 
} dpipe_t;
1) This pipe should connect some process with his child, for continuous communication.
2) Be careful with opened descriptors.
3) When one process is terminated, the other should also exit.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

typedef struct {
  int txd[2]; 
  int rxd[2]; 
} dpipe_t;

dpipe_t fd;

#define size  4096
char str[size];

int main(){
  char buf[size];
  int st1,st2;
  int child;
  st1=pipe(fd.txd);
  st2=pipe(fd.rxd);

  if(st1==-1 || st2==-1)
    exit(-1);
  child=fork();
  if(child==-1)
    exit(-1);
  else if(child){
    close(fd.txd[1]);
    close(fd.rxd[0]);

    for(;;){
    scanf("%s",&str);
    st2=read(fd.txd[0],buf,size);
    write(1,buf,st2);
    write(fd.rxd[1],str,strlen(str));
    if(strcmp("exit",buf)==0  || strcmp("exit",str)==0 ){
      close(fd.txd[0]);
      close(fd.rxd[1]);
      exit(0);
    }
    }
  }
  else{
    close(fd.txd[0]);
    close(fd.rxd[1]);

    for(;;){
    scanf("%s",&str);
    write(fd.txd[1],str,strlen(str));
    st1=read(fd.rxd[0],buf,size);
    write(1,buf,st1);
    if(strcmp("exit",buf)==0  || strcmp("exit",str)==0 ){
      close(fd.rxd[1]);
      close(fd.rxd[0]);
      exit(0);
    }
    }
  }
  return 0;
}
