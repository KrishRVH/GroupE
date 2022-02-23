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
	//free(buffer);
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

}

// NOTE: Priority 10 is used for game moves/logic, priority 9 is used for player structs
// and scoreboard usage

// Since buffer and message size is standard mq_attr.mq_msgsize, the ability to send different
// types is allowed sorted by priority level.

void recieveMsg()
{
	struct mq_attr attr;
	mq_getattr(mqd, &attr);
	//buffer = calloc(attr.mq_msgsize, 1);

	unsigned int priority = 0;
	if ((mq_receive(mqd, buffer, attr.mq_msgsize, &priority)) != -1)
	{
		// Player struct messsage. do something
		if (priority == 10)
		{
			struct Player* new_player = (struct Player*)buffer;
			printf("Player: %i, Prio: %i\n", new_player[0].score, priority);
		}
		// Game instruction message. do something
		if (priority == 9)
		{
			mq_receive(mqd, buffer, attr.mq_msgsize, &priority);
			printf("Message: %s, Prio: %i\n", buffer, priority);
		}
	}
	else
	{
		perror("ERROR");
	}
}

int newPlayer()
{
	int num_players = 3;
	struct Player* players = malloc(sizeof(Player) * num_players);

	printf("Players made");
	return 0;
}

// https://stackoverflow.com/questions/16328118/simple-tcp-server-with-multiple-clients-c-unix
void server()
{
	int sockfd, new_sockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	char buffer[MAX];
	int n;

	// Socket creation, port number
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char*)&serv_addr, sizeof(serv_addr));

	// Assigning IP, PORT
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(PORT);

	// Binds socket
	bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	clilen = sizeof(cli_addr);

	int pid;
	while (1)
	{
		new_sockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);

		pid = fork();
		
		// Child process, will contain game logic?
		if (pid == 0)
		{
			close(sockfd);
			n = read(new_sockfd, buffer, MAX-1);
			if (n < 0)
			{
				printf("Error reading from socket.");
			}
			printf("Message: %s\n", buffer);
			close(new_sockfd);
		}
		// Parent process for what?
		if (pid > 0)
		{
			close(new_sockfd);
		}
	}
}

void client()
{
	int sockfd, n;
    struct sockaddr_in serv_addr, cli;
	char buffer[MAX];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(PORT);

	connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
	bzero(buffer, MAX);
	n = write(sockfd, buffer, strlen(buffer));
}