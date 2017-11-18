#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<time.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/shm.h>
#include<pthread.h>
#include<math.h>
#include<string.h>
#define N 10000000


double F(double x){
    double y = -cos(x)+1;
    return y;
}

int main(int argc, char **argv){
    int i;
    int n=atoi(argv[1]);
    int fd = shmget(ftok("task.c",1),n*sizeof(int), 0666|IPC_CREAT);
    int *p = shmat(fd,NULL,0);
    int sum=0;
    for(i=0;i<n;i++) sum+=p[i];
    double ans = (double)sum/N;
    printf("Divergence:%lf\n",ans-F(1));
    shmdt(p);
    shmctl(shmget(ftok("task.c",1),n*sizeof(int), 0666),IPC_RMID,NULL);
    return 1;
}
