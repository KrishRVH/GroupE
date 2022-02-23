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
	mq_send(mqd, "HELLO 1", 8, 10);
	//mq_send(mqd, "HELLO 2", 8, 8);
}

// NOTE: Priority 10 is used for game moves/logic, priority 9 is used for player structs
// and scoreboard usage

// Since buffer and message size is standard mq_attr.mq_msgsize, the ability to send different
// types is allowed sorted by priority level.

void recieveMsg()
{
	mq_getattr(mqd, &attr);
	p_buffer = calloc(attr.mq_msgsize, 1);

	printf("# Of messages: %ld\n", attr.mq_curmsgs);

	unsigned int priority = 0;
	if ((mq_receive(mqd, p_buffer, attr.mq_msgsize, &priority)) != -1)
	{
		// Multiplayer waiting message, returns 1 if there is a player able to connect, returns 0 if no player or game is going on already
		if (priority == 10)
		{
			mq_receive(mqd, p_buffer, attr.mq_msgsize, &priority);
			printf("Message: %s, Prio: %i\n", p_buffer, priority);
			// Accept multiplayer connection and send message to start game between the two processes
		}
		// Game instruction message, this will c
		if (priority == 9)
		{
			mq_receive(mqd, p_buffer, attr.mq_msgsize, &priority);
			printf("Message: %s, Prio: %i\n", p_buffer, priority);
		}
		if (priority == 8)
		{
			mq_receive(mqd, p_buffer, attr.mq_msgsize, &priority);
			printf("Message: %s, Prio: %i\n", p_buffer, priority);
		}
		// Player struct information message, stores player info in array of structs
		//if (priority == 8)
		//{
		//	struct Player* new_player = (struct Player*)p_buffer;
		//	printf("Player: %i, Prio: %i\n", new_player[0].score, priority);
		//}
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

// Testing main method
int main()
{
	openMsgQueue();
	sendGameMsg();
	recieveMsg();
}