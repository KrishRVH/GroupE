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

// Mqueue globals
struct mq_attr attr; // Only used for buffer size/declaration in mqueue
char* p_buffer;

// Global storage for used words,

// Player struct
struct Player
{
	int score;
	char firstname[50];
	char lastname[50];
	char country[50];
	int num_words;
	int num_words_added;
	int player_turn;
} Player;

struct Player newPlayer(char *firstname, char *lastname, char *country)
{
	struct Player new_player;
	
	new_player.score = 0;
	strcpy(new_player.firstname, firstname);
	strcpy(new_player.lastname, lastname);
	strcpy(new_player.country, country);
	new_player.num_words = 0;
	new_player.num_words_added = 0;

	return new_player;
}

// Opens message queue, should only be ran once.
mqd_t openMsgQueue(char *queue_name)
{
	// Ensures message queue does not already exist and creates a new one
	mq_unlink(queue_name);
	mqd_t mqd = mq_open(queue_name, O_CREAT | O_RDWR, 0600, NULL);

	if (mqd == -1)
	{
		perror("mq_open");
		return NULL;
	}
	else
	{
		printf("MQ was opened \n");
		return mqd_t;
	}
}

void sendPlayerConnectMsg(mqd_t mqd)
{
	mq_send(mqd, "1", 1, 10);
}

int recievePlayerConnectMsg(mqd_t mqd)
{
	int prio = 10;
	mq_getattr(mqd, &attr);
	p_buffer = calloc(attr.mq_msgsize, 1);
	int num_msgs = attr.mq_curmsgs;
	
	if (num_msgs != 0)
	{
		if ((mq_receive(mqd, p_buffer, attr.mq_msgsize, &priority)) != -1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

// Send message for game instruction
void sendGameMsg(mqd_t mqd)
{
	mq_send(mqd, "HELLO", 6, 10);
	mq_send(mqd, "HELLO2", 7, 10);
	mq_send(mqd, "HELLO3", 7, 10);
}

void recieveMsg(mqd_t mqd)
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
			// Player struct information message, recieves following from player:
			// - Set of random alphabets (Global var)
			// - Current player score (Local for client fork)
			// - Other players score ***
			// - Words that have been used so far (Global var)
			if (priority == 8)
			{
				struct Player* new_player = (struct Player*)p_buffer;
				printf("Player: %i, Prio: %i\n", new_player[0].score, priority);
			}
		}
		num_msgs -= 1;
	}
	free(p_buffer);
}

// Test server socket code for multiple forked clients
int serverTest()
{
	// Opens POSIX message queue
	openMsgQueue();

	int sockfd, ret, newSocket;
	struct sockaddr_in serverAddr, newAddr;
	socklen_t addr_size;
	char buffer[MAX];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serverAddr, '\0', sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		printf("ERROR: Could not bind to port.\n");
		exit(1);
	}
	printf("CONSOLE: Binded to port %d\n", 4444);
    listen(sockfd, 10);

	// Player information
	char firstname[50];
    char lastname[50];
    char country[50];

	while(1)
	{
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0)
		{
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0)
        {
			close(sockfd);
			while(1)
			{
				recv(newSocket, buffer, 1024, 0);
                if(strcmp(buffer, "1") == 0)
				{
					// Single player game

					// POSIX queues
					mqd_t word_valid = openMsgQueue("/Word_valid");

                    // Receiving player information
                    recv(newSocket, buffer, 1024, 0);
                    strcpy(firstname, buffer);
                    bzero(buffer, sizeof(buffer));

                    recv(newSocket, buffer, 1024, 0);
                    strcpy(lastname, buffer);
                    bzero(buffer, sizeof(buffer));

                    recv(newSocket, buffer, 1024, 0);
                    strcpy(country, buffer);
                    bzero(buffer, sizeof(buffer));

                    // Create new player struct
                    struct Player added_player = newPlayer(firstname, lastname, country);

                    printf("First name: %s\n", added_player.firstname);
                    printf("Last name: %s\n", added_player.lastname);
                    printf("Country: %s\n", added_player.country);

					bzero(firstname, sizeof(firstname));
					bzero(lastname, sizeof(lastname));
					bzero(country, sizeof(country));

					// Game starts...
					int game_start = 1;
					while(game_start)
					{
						
					}
				}
                if(strcmp(buffer, "2") == 0)
				{
					// Multiplayer game

					// POSIX queues
					mqd_t waiting_players = openMsgQueue("/Waiting_players");

					// Receiving player information
                    recv(newSocket, buffer, 1024, 0);
                    strcpy(firstname, buffer);
                    bzero(buffer, sizeof(buffer));

                    recv(newSocket, buffer, 1024, 0);
                    strcpy(lastname, buffer);
                    bzero(buffer, sizeof(buffer));

                    recv(newSocket, buffer, 1024, 0);
                    strcpy(country, buffer);
                    bzero(buffer, sizeof(buffer));

                    // Create new player struct
                    struct Player added_player = newPlayer(firstname, lastname, country);

                    printf("First name: %s\n", added_player.firstname);
                    printf("Last name: %s\n", added_player.lastname);
                    printf("Country: %s\n", added_player.country);

					if (recievePlayerConnectMsg(waiting_players) == 1)
					{
						// Starts multiplayer game with other connected player
					}
					else
					{
						// Sends message to POSIX queue that this client is waiting.
						// Asks client if they want to wait after two minutes of waiting.
						sendPlayerConnectMsg(waiting_players);
					}

					

				}
                // Clean client exit
				if(strcmp(buffer, "3") == 0)
				{
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}
				else
				{
					//printf("Client: %s\n", buffer);
					//send(newSocket, buffer, strlen(buffer), 0);
					//bzero(buffer, sizeof(buffer));
				}
			}
		}
	}
	close(newSocket);
	return 0;
}

int main()
{

}