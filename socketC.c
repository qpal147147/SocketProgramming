#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<netinet/tcp.h>


int main(int argc,char *argv[]){
    int servfd,portNo = 6666,clifd;
    int clientLen;
    char sendBuffer[256],recBuffer[256];
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
    int flag = 1;
    setsockopt(servfd,IPPROTO_TCP,TCP_NODELAY,(char *)&flag,sizeof(flag));
	
    if(servfd >=0){
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("10.0.0.1");
        
		if(argc >=2){
			serv = gethostbyname(argv[1]);
			bcopy((char *)serv->h_addr,(char *)&serv_addr.sin_addr.s_addr,serv->h_length);
		}
		
		serv_addr.sin_port = htons(portNo);

		if(connect(servfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) >=0){
			while(1){
				printf("Enter Messenge:");
				bzero(sendBuffer,256);
				fgets(sendBuffer,256,stdin);
				
				
				if(strncmp(sendBuffer,"/FILE",5)==0){
					printf("Send file command start\n");
					n = write(servfd,sendBuffer,strlen(sendBuffer));
						
					for(int i = 0;i<100000000;i++);
						
					FILE *fp = fopen("hello","r");
					int rByte = 0;
					char fileBuffer[256] = {0};
						
					while((rByte = fread(fileBuffer,1,sizeof(fileBuffer),fp))>0){
						n = write(servfd,fileBuffer,strlen(fileBuffer));
						bzero(fileBuffer,sizeof(fileBuffer));
					}
						
					for(int i = 0;i<100000000;i++);
						
					char EOFBuffer[] = {EOF};
					write(servfd,EOFBuffer,sizeof(EOFBuffer));
					fclose(fp);
					printf("Send file command end\n");
				}
				else{
					n = write(servfd,sendBuffer,strlen(sendBuffer));
					printf("Send Messenge(%d):%s\n",n,sendBuffer);
				}
					
				bzero(recBuffer,sizeof(recBuffer));
				if((n =read(servfd,sendBuffer,256)) > 0){
					printf("Recv Messenge(%d):%s\n",n,sendBuffer);
				}
			}
		}
    }
    close(servfd);
    return 0;
}
