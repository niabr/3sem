#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include<stdlib.h>
#include<math.h>

#define N 1000

double F(double x){
    double y = -cos(x)+1;
    return y;
}

int main(int argc, char **argv)
{
    int sockfd,socktcp;
    int n, len, newlen;
    char sendline[1000], recvline[1000], tcpline[999]; 
    struct sockaddr_in servaddr,servaddr2, cliaddr, newaddr[100], tcpaddr[100]; 

    
    
    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror(NULL);
        exit(1);
    }

    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *) &cliaddr, 
    sizeof(cliaddr)) < 0){
        perror(NULL);
        close(sockfd);

        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(52001);
    if(inet_aton("127.0.0.1", &servaddr.sin_addr) == 0){
        printf("Invalid IP address\n");
        close(sockfd);

        exit(1);
    }

    bzero(&servaddr2, sizeof(servaddr2));
    servaddr2.sin_family = AF_INET;
    servaddr2.sin_port = htons(52001);
    if(inet_aton("127.0.0.1", &servaddr2.sin_addr) == 0){
        printf("Invalid IP address\n");
        close(sockfd);

        exit(1);
    }

    
    int one=1;
    setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&one,sizeof(one));

    strcpy(sendline,"hello");
    

    if(sendto(sockfd, sendline, strlen(sendline)+1, 
	      0, (struct sockaddr *) &servaddr, 
	      sizeof(servaddr)) < 0){
	perror(NULL);
	close(sockfd);
	exit(1);
    }
    sleep(5);
    

    int k=0;
    while(n!=-1){
	n = recvfrom(sockfd, recvline, 1000,MSG_DONTWAIT ,(struct sockaddr *) &(newaddr[k]), &newlen);

	printf("f%s\n", recvline);
	k++;
    }
    printf("%d\n",k);

    int nn=N/k;

    sprintf(sendline,"%d",nn);

    if(sendto(sockfd, "bye", 4, 
	      0, (struct sockaddr *) &servaddr, 
	      sizeof(servaddr)) < 0){
	perror(NULL);
	close(sockfd);
	exit(1);
    }
    sleep(5);

    
    int sum=0;
    n=0;
     /* while(n!=-1){ */
     /* 	n = recvfrom(sockfd, recvline, 1000,MSG_DONTWAIT ,(struct sockaddr *) &(newaddr[k]), &newlen); */

	/* puts("***"); */
	/* puts(recvline); */
	/* //printf("%s\n", recvline); */
	/* puts("***"); */
    int i=0;
    socktcp=socket(PF_INET, SOCK_STREAM, 0);
    // for(i=0;i<k;i++){
	bzero(&tcpaddr[i],sizeof(tcpaddr[i]));
	tcpaddr[i].sin_family=AF_INET;
	tcpaddr[i].sin_port=htons(52001);
	tcpaddr[i].sin_addr=newaddr[i].sin_addr;

	/* connect(socktcp, (struct sockaddr *) &tcpaddr[i], sizeof(tcpaddr[i])); */
	/* sprintf(tcpline,"%d",nn); */
	/* n=write(socktcp,tcpline,strlen(tcpline)+1); */
	/* n=read(socktcp,tcpline,999); */

	connect(socktcp, (struct sockaddr *) &servaddr, sizeof(servaddr));
	sprintf(tcpline,"%d",nn);
	n=write(socktcp,tcpline,strlen(tcpline)+1);
	n=read(socktcp,tcpline,999);
	sum+=atoi(tcpline);	


	connect(socktcp, (struct sockaddr *) &servaddr2, sizeof(servaddr2));
	sprintf(tcpline,"%d",nn);
	n=write(socktcp,tcpline,strlen(tcpline)+1);
	n=read(socktcp,tcpline,999);
	sum+=atoi(tcpline);	
	
	//}

//}
     printf("%d\n",sum);
     double ans = (double)(sum)/N;
	printf("Divergence:%lf\n",ans-F(1));

	//for(i=0;i<k;i++) printf("%s\n",tcpaddr[i].sin_addr);
    close(sockfd);
    close(socktcp);
    return 0;
}
