// Generic includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Socket/network includes
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>

#include <pthread.h>

// Mqueue include
#include <mqueue.h>

// Error handling
#include <errno.h>

// Constants
#define MAX 50
#define PORT 8000
#define SA struct sockaddr

// https://github.com/nikhilroxtomar/Multiple-Client-Server-Program-in-C-using-fork
int main()
{
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0)
	{
		printf("ERROR: Cannot create client socket.\n");
		exit(1);
	}
	printf("CONSOLE: Created client socket.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		printf("ERROR: Cannot connect to server.\n");
		exit(1);
	}
	printf("CONSOLE: Connected to Server.\n");

	while(1)
	{
		printf("Welcome to the word game.\n");
        printf("Please choose an option below (1 for Singleplayer, 2 for Multiplayer, 3 for exit)\n");
        printf("1) Singleplayer\n");
        printf("2) Multiplayer\n");
        printf("3) Exit\n");
        printf("> ");

		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);

        if(strcmp(buffer, "1") == 0)
		{
            printf("\nSingle Player Mode\n");
            printf("Enter your first name: ");
            scanf("%s", &buffer[0]);
            send(clientSocket, buffer, strlen(buffer), 0);

            printf("\nEnter your last name: ");
            scanf("%s", &buffer[0]);
            send(clientSocket, buffer, strlen(buffer), 0);
            
            printf("\nEnter your country: ");
            scanf("%s", &buffer[0]);
            send(clientSocket, buffer, strlen(buffer), 0);
        }

        if(strcmp(buffer, "1") == 0)
		{

		}

		if(strcmp(buffer, "3") == 0)
		{
			close(clientSocket);
			printf("Exiting game. Closing application.\n");
			exit(1);
		}

        recv(clientSocket, buffer, 1024, 0);
        printf("Server: \t%s\n", buffer);
	}
	return 0;
}