#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

int main(int argc, char** argv){
    int fd,fd2;
    long long count;
    struct stat buf;
    fd=open(argv[1],O_RDONLY);
    fd2=open(argv[2],O_RDWR|O_CREAT,0666);
    fstat(fd,&buf);

    count=buf.st_size;
    char *ptr,*ptr2;
    ftruncate(fd2,count);
    
    ptr=mmap(NULL,count,PROT_READ,MAP_SHARED,fd,0);
    close(fd);
    ptr2=mmap(NULL,count,PROT_READ|PROT_WRITE,MAP_SHARED,fd2,0);

    memcpy(ptr2,ptr,count);
    close(fd2);
    munmap(ptr,count);
    munmap(ptr2,count);
    return 0;
}
