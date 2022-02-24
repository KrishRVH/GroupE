// Author: Adam Loeckle
// Responsibilities:
// - POSIX Queue/Server side messaging
//
// References:
// https://linux.die.net/man/3/

#include "Main.h"

// Opens message queue, should only be ran once.
void openMsgQueue()
{
	// Ensures message queue does not already exist and creates a new one
	mq_unlink("/Message_Queue");
	mqd = mq_open("/Message_Queue", O_CREAT | O_RDWR, 0600, NULL);

	if (mqd == -1)
	{
		perror("mq_open");
		exit(1);
	}
	else
	{
		printf("MQ was opened \n");
	}
}

// Closes Message queue, if using mq_close(mqd) instead of mq_unlink ensure that the
// mqd is already initialized 
void closeMsgQueue()
{
	free(p_buffer);
	mq_unlink("/Message_Queue");
}

// Send message of player struct
void sendPlayerMsg(struct Player player_input)
{
	mq_send(mqd, (const char*)&player_input, sizeof(struct Player), 10);
}

// Send message for game instruction
void sendGameMsg()
{
	mq_send(mqd, "HELLO", 6, 10);
	mq_send(mqd, "HELLO2", 7, 9);
}

// NOTE: Priority 10 is used for game moves/logic, priority 9 is used for player structs
// and scoreboard usage

// Since buffer and message size is standard mq_attr.mq_msgsize, the ability to send different
// types is allowed sorted by priority level.

// These messages get processed by priority, this method should be called inside of the while loop
// of the server to constantly recieve messages from the queue.

void recieveMsg()
{
	mq_getattr(mqd, &attr);
	p_buffer = calloc(attr.mq_msgsize, 1);

	printf("# Of messages: %ld\n", attr.mq_curmsgs);
	int num_msgs = attr.mq_curmsgs;
	
	unsigned int priority = 0;
	while (num_msgs != 0)
	{
		if ((mq_receive(mqd, p_buffer, attr.mq_msgsize, &priority)) != -1)
		{
			// Multiplayer waiting message, returns 1 if there is a player able to connect, returns 0 if no player or game is going on already
			if (priority == 10)
			{
				printf("Message: %s, Prio: %i\n", p_buffer, priority);
				// Accept multiplayer connection and send message to start game between the two processes
			}
			// Game instruction message, this will c
			if (priority == 9)
			{
				printf("Message: %s, Prio: %i\n", p_buffer, priority);
			}
			//if (priority == 8)
			//{
			//	printf("Message: %s, Prio: %i\n", p_buffer, priority);
			//}
			// Player struct information message, stores player info in array of structs
			//if (priority == 8)
			//{
			//	struct Player* new_player = (struct Player*)p_buffer;
			//	printf("Player: %i, Prio: %i\n", new_player[0].score, priority);
			//}
		}
		num_msgs -= 1;
	}
}

struct Player newPlayer(char *firstname, char *lastname, char *country)
{
	struct Player new_player;
	
	strcpy(new_player.firstname, firstname);
	strcpy(new_player.lastname, lastname);
	strcpy(new_player.country, country);
	new_player.num_words = 0;
	new_player.num_words_added = 0;
	new_player.score = 0;

	return new_player;
}

void * cientThread(void *arg)
{
	printf("In thread\n");
	char message[1000];
  	char buffer[1024];
	int clientSocket;
  	struct sockaddr_in serverAddr;
  	socklen_t addr_size;
	
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7799);
	serverAddr.sin_addr.s_addr = inet_addr("localhost");
  	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	
	addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
    strcpy(message,"Hello");

	if( send(clientSocket , message , strlen(message) , 0) < 0)
    {
            printf("Send failed\n");
    }

    //Read the message from the server into the buffer
    if(recv(clientSocket, buffer, 1024, 0) < 0)
    {
       printf("Receive failed\n");
    }
    //Print the received message
    printf("Data received: %s\n",buffer);
    close(clientSocket);
    pthread_exit(NULL);
}

void client()
{
	int i = 0;
	pthread_t tid[51];
	while(i< 50)
	{
		if( pthread_create(&tid[i], NULL, cientThread, NULL) != 0 )
			printf("Failed to create thread\n");
		i++;
	}
	sleep(20);
	i = 0;
	while(i< 50)
	{
		pthread_join(tid[i++],NULL);
		printf("%d:\n",i);
	}
}

// Test server socket code for multiple forked clients
int main(){

	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Client: %s\n", buffer);
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	close(newSocket);


	return 0;
}