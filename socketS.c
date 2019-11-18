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
    char recBuffer[256]={0}, sendBuffer[]={"Thank You!"};
    struct sockaddr_in serv_addr,cli_addr;
    int n;
    
    //chat
    int fdmax = -1,retval = 0;
    fd_set tmpSet,master;
    FD_ZERO(&tmpSet);
    FD_ZERO(&master);
    
    servfd = socket(AF_INET,SOCK_STREAM,0);

    if(servfd >= 0){
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serv_addr.sin_port = htons(portNo);

        if(bind(servfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) >= 0){
            listen(servfd,5);
            FD_SET(servfd,&master);
            fdmax = servfd;
            
            printf("Server start listening!\n");
            
            //server main block
            while(1){
            	tmpSet = master;
            	retval = select(fdmax+1,&tmpSet,NULL,NULL,NULL);
            	
            	if(retval == 0){
            		continue;
            	}
            	else if(retval == -1){
            		printf("Select error!");
            	}
            	else{
            		
            	}
            }
            /*
			clientLen = sizeof(cli_addr);
            clifd = accept(servfd,(struct sockaddr*)&cli_addr,&clientLen);
            
            //test ID multiplex
            scanf("%s",sendBuffer);
            printf("send Message:%s\n",sendBuffer);
            n = write(clifd,sendBuffer,strlen(sendBuffer));
            
            while(1){
                bzero(recBuffer,sizeof(recBuffer));
                
                if((n =read(clifd,recBuffer,sizeof(recBuffer))) > 0){
				printf("Messenge:%s\n",recBuffer);
  		    		
					if(strncmp(recBuffer,"/FILE",5)==0){
						printf("recv file ommand start\n");
						FILE *fp = fopen("recv.txt","w");
						int rByte = 0;
							
						while((rByte = read(clifd,recBuffer,sizeof(recBuffer)))>0){
							char EOFBuffer[] = {EOF};
										
							if(strncmp(recBuffer,EOFBuffer,1)==0){
							break;
							}
									
							printf("file:%s\n",recBuffer);
							fwrite(recBuffer,strlen(recBuffer),1,fp);
							bzero(recBuffer,sizeof(recBuffer));
						}
						fclose(fp);
							
						char fileEnd[] = {"File End"};
						n = write(clifd,fileEnd,strlen(fileEnd));
						printf("recv file command end\n");
				
					}
					else{
						n = write(clifd,sendBuffer,strlen(sendBuffer));
					}
				}
			}*/
		}
    }
}

