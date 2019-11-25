#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
typedef struct{
	int id;
	char name[20];
}user;

int main(){
    int servfd,portNo=7777,clifd;
    int clientLen;
	user users[20];
    char recBuffer[256]={0}, sendBuffer[]={"Thank You!"};
    struct sockaddr_in serv_addr,cli_addr;
    int n;
    
	//initialization user name
	for(int index = 0;index<20;index++){
		users[index].id = 0;
		snprintf(users[index].name,sizeof(users[index].name),"User %d",index-3);	
	}
	
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
            		for(int i = 0;i<=fdmax;i++){
            			if(FD_ISSET(i,&tmpSet)){
            				if(i == servfd){
            					clientLen = sizeof(cli_addr);
            					clifd = accept(servfd,(struct sockaddr*)&cli_addr,&clientLen);
            					if(clifd != -1){
            						FD_SET(clifd,&master);
            						if(clifd > fdmax) fdmax = clifd;
            						printf("NO[%d] => %s online!\n",clifd,users[clifd].name);
            						/* and then broadcast this news */
            						
            						fd_set broSet = master;
            						for(int j = 0;j<=fdmax;j++){
            							if(FD_ISSET(j,&broSet)){
            								if(j!=servfd && j != clifd){
            									char writeBuff[256] = {0};
            									snprintf(writeBuff,sizeof(writeBuff), " %s online!",
            									users[clifd].name);
            									write(j,writeBuff,strlen(writeBuff));
            									
            								}
            							}
            						}
            					}
            				}
            				else{ //client say something or disconnection
            					if((n=read(i,recBuffer,sizeof(recBuffer))) <= 0){
            						if(n <= 0){
            							printf("NO[%d] => %s leave!\n",i,users[i].name);
            							/* and then broadcast this news */
            							
            							fd_set broSet = master;
		        						for(int j = 0;j<=fdmax;j++){
		        							if(FD_ISSET(j,&broSet)){
		        								if(j!=servfd && j != i){
		        									char writeBuff[256] = {0};
		        									snprintf(writeBuff,sizeof(writeBuff), " %s leave!",
		        									users[i].name);
		        									write(j,writeBuff,strlen(writeBuff));
		        									
		        								}
		        							}
		        						}
            						}
            						
            						close(i);
            						FD_CLR(i,&master);
            					}
            					else{
            						fd_set broSet = master;
            						for(int j = 0;j<=fdmax;j++){
            							if(FD_ISSET(j,&broSet)){
            								if(j!=servfd && j != i){
            									char writeBuff[256] = {0};
            									snprintf(writeBuff,sizeof(writeBuff), " No[%d]: %s",
            									i, recBuffer);
            									write(j,writeBuff,strlen(writeBuff));
            									
            								}
            							}
            						}
            					}
            					
            					bzero(recBuffer,sizeof(recBuffer));
            				}
            			}
            		}
            	}
            }
		}
		close(clifd);
    }
	return 0;
}

