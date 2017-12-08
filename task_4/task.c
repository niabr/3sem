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

    buf.mtype=1;
    key=ftok("task.c",1);
    id=msgget(key,0666|IPC_CREAT);

    fd=open(argv[1],O_RDONLY);
    while(count!=0){
	count=read(fd,buf.mtext,8192);
	msgsnd(id,&buf,count,0);
    }
    buf.mtype=255;
    msgsnd(id,&buf,0,0);
    close(fd);
    return 0;
}
