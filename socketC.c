#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<netinet/tcp.h>

int main(int argc, char *argv[]) {
	int servfd, portNo = 6666, clifd;
	int clientLen;
	char sendBuffer[256], recBuffer[256];
	struct sockaddr_in serv_addr;
	struct hostent *serv;
	fd_set read_fds;
	int fdmax = -1;
	int retval = 0;
	int n;

	//print ipPort & refence
	for (int i = 0; i<argc; i++) {
		printf("arg:%d argv:%s\n", argc, argv[i]);
	}

	//set ipPort
	if (argc >= 2) {
		portNo = atoi(argv[2]);
	}

	servfd = socket(AF_INET, SOCK_STREAM, 0);
	int flag = 1;
	setsockopt(servfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));

	if (servfd >= 0) {
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr("10.0.0.1");

		if (argc >= 2) {
			serv = gethostbyname(argv[1]);
			bcopy((char *)serv->h_addr, (char *)&serv_addr.sin_addr.s_addr, serv->h_length);
		}

		serv_addr.sin_port = htons(portNo);

		if (connect(servfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) >= 0) {
			while (1) {
				FD_ZERO(&read_fds);
				FD_SET(0, &read_fds);
				fdmax = 0;
				FD_SET(servfd, &read_fds);
				fdmax = servfd;
				retval = select(fdmax + 1, &read_fds, NULL, NULL, NULL);
				if (retval == 0) {
					continue;
				}
				else {
					if (FD_ISSET(servfd, &read_fds)) {
						bzero(recBuffer, sizeof(recBuffer));
						if ((n = read(servfd, recBuffer, sizeof(recBuffer)))>0) {
							printf("\nRecv Message(%d):%s\n", n, recBuffer);
						}
					}
					if (FD_ISSET(0, &read_fds)) {
						//printf("Enter Messenge:");
						bzero(sendBuffer, 256);
						fgets(sendBuffer, 256, stdin);

						if (strncmp(sendBuffer, "/FILE", 5) == 0) {
							//first command
							char *fileName = strchr(sendBuffer,'[') + 1;
							strtok(fileName,"]");
							
							FILE *fp = fopen(fileName, "r");
							
							if(fp != NULL){
								printf("Send file command start\n");
								n = write(servfd, sendBuffer, strlen(sendBuffer));

								for (int i = 0; i<100000000; i++);

							
								int rByte = 0;
								char fileBuffer[256] = { 0 };

								while ((rByte = fread(fileBuffer, 1, sizeof(fileBuffer), fp))>0) {
									n = write(servfd, fileBuffer, strlen(fileBuffer));
									bzero(fileBuffer, sizeof(fileBuffer));
								}

								for (int i = 0; i<100000000; i++);

								char EOFBuffer[] = { EOF };
								write(servfd, EOFBuffer, sizeof(EOFBuffer));
								fclose(fp);
								
								printf("Send file command end\n");
							}
							else{
								printf("File not found!\n");
							}
						}
						else if(strncmp(sendBuffer, "/GAME ALL", 9) == 0){
							//second command
							printf("[/GAMEG] is guess\n[/GAMEM] is mora\n");
						}
						else if(strncmp(sendBuffer, "/GAMEG", 6) == 0){
							//third command
							int ans = 0, input = 0, min = 1, max = 100;
							srand(time(NULL));
							ans = (rand() % (max - min + 1)) + 1;
							printf("Please enter number(1~100):\n");

							
							while (input != -1) {
								scanf("%d", &input);
								if ((input > max || input < min) && input != -1) {
									printf("Please enter again!\n");
								}
								else if (input > ans && input != -1) {
									max = input;
									printf("Range is [%d~%d],Please enter again!\n",min,max);
								}
								else if (input < ans && input != -1) {
									min = input;
									printf("Range is [%d~%d],Please enter again!\n", min, max);
								}
								else if(input != -1){
								 printf("Correct!\n");
								 input = -1;
								}
							}
						}
						else if(strncmp(sendBuffer, "/GAMEM", 6) == 0){
							//four command
							int array[] = {2,0,5};
							int server = array[rand()%2];
							int input = 0;
							printf("Please enter number(0 is fist,2 is scissors,5 is cloth):");
							
							while(input != -1){
								scanf("%d",&input);
								
								if(input == 0 || input == 2 || input == 5){
									if(server != input){
										if(server == 2){
											if(input == 0){	
												printf("You Win!\n");
											}
											else if(input == 5){
												printf("You Lose!\n");
											}
										}
										else if(server == 0){
											if(input == 2){	
												printf("You Lose!\n");
											}
											else if(input == 5){
												printf("You Win!\n");
											}
										}
										else if(server == 5){
											if(input == 2){	
												printf("You Win!\n");
											}
											else if(input == 0){
												printf("You Lose!\n");
											}
										}
									
										input = -1;
									}
									else if(server == input){
										printf("Draw!Please enter again:\n");
										server = array[rand()%2];
									}
								}
								else{
									printf("Please enter again!\n");
								}
							}
						}
						//five command
						else if(strncmp(sendBuffer, "/IMG ALL", 7) == 0){
							printf("%s\n","[/IMG 1] is (っ・Д・)っ");
							printf("%s\n","[/IMG 2] is (･ω´･ )");
							printf("%s\n","[/IMG 3] is (`・ω・´)");
							printf("%s\n","[/IMG 4] is ,,Ծ‸Ծ,,");
							printf("%s\n","[/IMG 5] is (╬ﾟдﾟ)╭∩╮");
							printf("%s\n","[/IMG 6] is _(┐「ε:)_");
							printf("%s\n","[/IMG 7] is (*´∀`)~♥");
							printf("%s\n","[/IMG 8] is (๑•́ ₃ •̀๑)");
							printf("%s\n","[/IMG 9] is (ㆆᴗㆆ)");
							printf("%s\n","[/IMG 10] is (◞‸◟)");
						}
						//six command
						else if(strncmp(sendBuffer, "/help", 5) == 0){
							printf("%s\n","/FILE [fileName] : upload file");
							printf("%s\n","/GAME ALL : show all game");
							printf("%s\n","/IMG ALL : show all img");
						}
						else {
							n = write(servfd, sendBuffer, strlen(sendBuffer));
							printf("Send Messenge(%d):%s\n", n, sendBuffer);
						}

						bzero(recBuffer, sizeof(recBuffer));
					}
				}
			}
		}
	}
	close(servfd);
	return 0;
}
