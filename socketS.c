#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdbool.h>
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
	serv_addr.sin_addr.s_addr = inet_addr("10.0.0.1");//Multiplayer 10.0.0.1
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
								else {
									/*private*/
									char *result = NULL;
									char s[1024];
									bool towho[20]={0};
									char msg[1000] = {""};
									bool isMsg = false;
									strncpy(s,recBuffer,sizeof(recBuffer));
									if (!strncmp(s, "/private", 8)) {/*check user*/
										isMsg=true;
										result = strtok(s," ");//remove /private
										result = strtok(NULL," ");	//  /private User1 User2 User3/ hi
										/*checkuser*/
										bool isEnd=false;
										while(result != NULL){
											int i=0;
											for(i=0;i<20;i++){
												if(result[strlen(result)-1]=='/'){
													if (!strncmp(users[i].name, result, strlen(result)-1)){
														towho[i]=1;
														isEnd=true;
														break;
													}
												}
												else if (!strncmp(users[i].name, result, strlen(users[i].name))){
													towho[i]=1;
												}
											}
											strncpy(msg,result,sizeof(result));
											result = strtok(NULL," ");
											if(isEnd){
												strncpy(msg,result,sizeof(result));
												result = strtok(NULL," ");
												break;
											}
										}
									}
									//five command
									else if(!strncmp(s, "/IMG", 3)){
										char *img = NULL;
										img = strtok(s," ");//remove /img
										img = strtok(NULL," ");// /img 1
										int imgNo = atoi(img);
										
										if(imgNo == 1){
											strcpy(recBuffer,"(っ・Д・)っ");
										}
										else if(imgNo == 2){
											strcpy(recBuffer,"(･ω´･ )");
										}
										else if(imgNo == 3){
											strcpy(recBuffer,"(`・ω・´)");
										}
										else if(imgNo == 4){
											strcpy(recBuffer,",,Ծ‸Ծ,,");
										}
										else if(imgNo == 5){
											strcpy(recBuffer,"(╬ﾟдﾟ)╭∩╮");
										}
										else if(imgNo == 6){
											strcpy(recBuffer,"_(┐「ε:)_");
										}
										else if(imgNo == 7){
											strcpy(recBuffer,"(*´∀`)~♥");
										}
										else if(imgNo == 8){
											strcpy(recBuffer,"(๑•́ ₃ •̀๑)");
										}
										else if(imgNo == 9){
											strcpy(recBuffer,"(ㆆᴗㆆ)");
										}
										else if(imgNo == 10){
											strcpy(recBuffer,"(◞‸◟)");
										}
									}
									//first command
									else if(strstr(recBuffer,"/FILE")!=NULL){
										char *fileName = strchr(recBuffer,'[') + 1;
										strtok(fileName,"]");
										
										printf("recv file command start\n");
										
										FILE *fp = fopen(strcat(fileName,"Copy"),"w");
										int rByte = 0;
										
										while((rByte = read(clifd,recBuffer,sizeof(recBuffer)))>0){
											char EOFBuffer[] = {EOF};
											
											if(strncmp(recBuffer,EOFBuffer,1)==0){
												break;
											}
											
											printf("file:%s\n",recBuffer);
											fwrite(recBuffer,1,sizeof(recBuffer),fp);
											bzero(recBuffer,sizeof(recBuffer));
										}
										fclose(fp);
										
										char fileEnd[] = {"File End"};
										n = write(clifd,fileEnd,strlen(fileEnd));
										printf("recv file command end\n");
									}
									
									//six command
									else if(strstr(recBuffer,"/DFILE")!=NULL){
										char *fileName = strchr(recBuffer,'[') + 1;
										strtok(fileName,"]");
										
										FILE *fp = fopen(fileName, "r");
										if(fp != NULL){
											printf("Rec file command start\n");
											
											for (int i = 0; i<100000000; i++);
											
											int rByte = 0;
											char fileBuffer[256] = { 0 };
											
											while ((rByte = fread(fileBuffer, 1, sizeof(fileBuffer), fp))>0) {
												n = write(clifd, fileBuffer, strlen(fileBuffer));
												bzero(fileBuffer, sizeof(fileBuffer));
											}
											
											for (int i = 0; i<100000000; i++);
											fclose(fp);
										}
									}
									
									if(isMsg) {
										/*private send message*/
										fd_set broSet = master;
										for(int j=0;j<=fdmax;j++){
											if(FD_ISSET(j, &broSet)){
												if(j!=servfd && j!=i && towho[j] && isMsg) {//servfd is server, i is client who send.
													char writeBuff[256] ={0};
													snprintf(writeBuff, sizeof(writeBuff), " %s: %s", users[i].name, msg);
													write(j, writeBuff, strlen(writeBuff));
												}
											}
										}
									} 
									else {
										/*broadcast*/
										fd_set broSet = master;
										for(int j=0;j<=fdmax;j++){
											if(FD_ISSET(j, &broSet)){
												if(j!=servfd && j!=i) {//servfd is server, i is client who send.
													char writeBuff[256] ={0};
													snprintf(writeBuff, sizeof(writeBuff), " %s: %s", users[i].name, recBuffer);
													write(j, writeBuff, strlen(writeBuff));
												}
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
