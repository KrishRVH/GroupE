// Author: Adam Loeckle

#include "Main.h"

void server()
{
	int new_socket;
	int new_connection;
	int len;

	struct sockaddr_in server_address;
	struct sockaddr_in cli;

	new_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (new_socket == -1)
	{
		printf("Socket creation failed \n");
	}
	else
	{
		printf("Socket created. \n");
	}

	bzero(&server_address, sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(PORT);

	if ((bind(new_socket, (SA*)&server_address, sizeof(server_address))) != 0)
	{
		printf("Socket bind failed \n");
	}
	else
	{
		printf("Socket binded successfully \n");
	}

	if ((listen(new_socket, 5)) != 0)
	{
		printf("Listen failed \n");
	}
	else
	{
		printf("Server listening \n");
	}

	len = sizeof(cli);

	new_connection = accept(new_socket, (SA*)&cli, &len);
	if (new_connection < 0)
	{
		printf("Server accept failed \n");
	}
	else
	{
		printf("Server accepted the client \n");
	}

	close(new_socket);
}

// Game logic, handles recieving/sending messages
void game()
{

}

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
	free(buffer);
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
	buffer = calloc(attr.mq_msgsize, 1);

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

int player()
{
	int num_players = 3;
	struct Player* players = malloc(sizeof(Player) * num_players);

	printf("Players made");
	return 0;
}