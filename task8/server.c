#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<time.h>
#include<math.h>

int N;

double f(double x){
    double y = sin(x);
    return y;
}

int arg=0;
char ansline[999];

void calculations(int num){

	    srand(time(NULL));
	    int j;
	    double x;
	    double y;
	    arg=0;
	    for(j=0;j<=N;j++){
		x=(double)rand()/RAND_MAX;
		y=(double)rand()/RAND_MAX;
		if(y<=f(x))
		    arg++;
	    }
	    sprintf(ansline,"%d\0",arg);
	    return;
}

int main()
{
    char tcpline[1000];
    int sockfd,socktcp;
    int clilen, n; 
    char line[1000]; 
    struct sockaddr_in servaddr, cliaddr, tcpaddr; 

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(52001);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);


    
    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror(NULL);    
        exit(1);
    }

    if((socktcp = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror(NULL);    
        exit(1);
    }    

    if(bind(sockfd, (struct sockaddr *) &servaddr, 
    sizeof(servaddr)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }


    bzero(&tcpaddr, sizeof(tcpaddr));
    tcpaddr.sin_family = AF_INET;
    tcpaddr.sin_port = htons(52001);
    tcpaddr.sin_addr.s_addr = servaddr.sin_addr.s_addr;

    bind(socktcp, (struct sockaddr *) &tcpaddr, sizeof(tcpaddr));
    listen(socktcp,5);



    
    while(1) {

        clilen = sizeof(cliaddr);
        
        if((n = recvfrom(sockfd, line, 999, 0, 
        (struct sockaddr *) &cliaddr, &clilen)) < 0){
            perror(NULL);
            close(sockfd);
            exit(1);
        }
       
        printf("%s\n", line);
        
	if(strcmp(line,"hello")==0){
	    if(sendto(sockfd, line, strlen(line), 0, 
		      (struct sockaddr *) &cliaddr, clilen) < 0){
		perror(NULL);
		close(sockfd);
		exit(1);
	    }
	}
	else if(strcmp(line,"bye")==0) {
	    close(sockfd);
	    break;
	}
	else {
	    
	    if(sendto(sockfd, line, strlen(line), 0, 
		      (struct sockaddr *) &cliaddr, clilen) < 0){
		perror(NULL);
		close(sockfd);
		exit(1);
	    }
	    printf("%s\n",line);
	}
    
    }


    while(1){
	clilen=sizeof(cliaddr);
	int newsockfd;
	newsockfd=accept(socktcp, (struct sockaddr *) &cliaddr, &clilen);

	while((n=read(newsockfd,tcpline,999))>0){
	    N=atoi(tcpline);
	    calculations(N);
	    n=write(newsockfd,ansline,strlen(ansline));
	}
	close(newsockfd);

    }
    close(socktcp);
    return 0;
}
