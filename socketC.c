#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>


int main(int argc,char *argv[]){
    int servfd,portNo = 6666,clifd;
    int clientLen;
    char sendBuffer[256];
    struct sockaddr_in serv_addr;
    struct hostent *serv;
    int n;

    //print ipPort & refence
    for(int i = 0;i<argc;i++){
        printf("arg:%d argv:%s\n",argc,argv[i]);
    }

    //set ipPort
    if(argc>=2){
        portNo = atoi(argv[2]);
    }

    servfd = socket(AF_INET,SOCK_STREAM,0);
    if(servfd >=0){
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        
	if(argc >=2){
	    serv = gethostbyname(argv[1]);
	    bcopy((char *)serv->h_addr,(char *)&serv_addr.sin_addr.s_addr,
            serv->h_length);
	}
	serv_addr.sin_port = htons(portNo);

        if(connect(servfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) >=0){
            while(1){
                printf("Enter Messenge:");
                bzero(sendBuffer,256);
                fgets(sendBuffer,256,stdin);
                n = write(servfd,sendBuffer,strlen(sendBuffer));
                printf("Send Messenge(%d):%s\n",n,sendBuffer);
                
		        if((n =read(servfd,sendBuffer,256)) > 0){
  		            printf("Recv Messenge(%d):%s\n",n,sendBuffer);
		        }
            }
        }
    }

    return 0;
}
