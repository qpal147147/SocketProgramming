#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(){
    int servfd,portNo=6666,clifd;
    int clientLen;
    char recBuffer[256], sendBuff[]={"Thank You!"};
    struct sockaddr_in serv_addr,cli_addr;
    int n;

    servfd = socket(AF_INET,SOCK_STREAM,0);
    if(servfd >= 0){
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serv_addr.sin_port = htons(portNo);

        if(bind(servfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) >= 0){
            listen(servfd,1);
            clifd = accept(servfd,(struct sockaddr*)&serv_addr,&serv_addr);
            while(1){
                bzero(recBuffer,256);
                if((n =read(clifd,recBuffer,256)) > 0){
  		    printf("Messenge:%s\n",recBuffer);
		    n = write(clifd,sendBuff,strlen(sendBuff));
		}
            }
        }
    }
}
