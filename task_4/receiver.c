#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

struct msgbuf{
    long mtype;
    char mtext[8192];
} buf;
    
int main(int argc, char** argv){
    int id,fd;
    int count=1;
    //char buf[8192];
    key_t key;

    key=ftok("task.c",1);
    id=msgget(key,0666);
    fd=open(argv[1],O_WRONLY|O_CREAT,0666);

    while(count!=0){
	count=msgrcv(id,&buf,8192,0,0);
	write(fd,buf.mtext,count);
    }
    msgctl(id,IPC_RMID,NULL);
    close(fd);
    return 0;
}
