#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

void *doRecieving(void *sockID)
{

	int clientSocket = *((int *)sockID);

	while (1)
	{

		char data[1024];
		int read = recv(clientSocket, data, 1024, 0);
		if (read <= 0)
		{
			printf("\033[1;33m[x] Server closed!!\033[0m\n\n");
			exit(1);
		}
		data[read] = '\0';
		printf("\033[1;32m> %s\033[0m\n", data);
	}
}

void sigintHandler(int sig_num)
{

	signal(SIGINT, sigintHandler);
	printf("\n[+] Cannot be terminated using \033[1;31mCtrl+C\033[0m Type \033[1;32m'EXIT'\033[0m to terminate.\n");
	fflush(stdout);
}
int main()
{
	printf("\033[1;33m ======== Welcome to chat-room  ======== \033[0m\n");

	int identifier = 1;
	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(10000);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
		return 0;

	printf("\033[1;32m[+]Connection established!!\033[0m\n\033[1;33m Command List\033[0m\n\033[1;36m> Type '<client id> <message>' to unicast chating.\n> Type 'BROADCAST <message>' for broadcasting.\n> Type 'LIST' to view no: of active clients.\n> Type 'EXIT' to leave from chat-room.\n\033[0m");

	pthread_t thread;
	pthread_create(&thread, NULL, doRecieving, (void *)&clientSocket);

	char input[1024];
	signal(SIGINT, sigintHandler);
	while (1)
	{
		scanf("%s", input);

		if (strcmp(input, "EXIT") == 0)
		{

			send(clientSocket, input, 1024, 0);
			break;
		}

		else if (strcmp(input, "BROADCAST") == 0)
		{

			send(clientSocket, input, 1024, 0);
			char msg[1024];
			bzero(msg, sizeof(msg));
			gets(msg);

			send(clientSocket, msg, 1024, 0);
		}
		else if (strlen(input) <= 2)
		{

			int j, flag = 0;;
			j = strlen(input);
			while (j--)
			{
				if (input[j] > 47 && input[j] < 58){
					flag ++;
					continue;
				}

				
			}

			if (flag == strlen(input)){
			send(clientSocket, input, 1024, 0);
			bzero(input, sizeof(input));
			//scanf("%s", input);
            gets(input);
            //printf("%s", input);
			send(clientSocket, input, 1024, 0);

			}
		}
		else
		{
			send(clientSocket, input, 1024, 0);
		}

		bzero(input, sizeof(input));
	}
    printf("EXIT");
	close(clientSocket);
}