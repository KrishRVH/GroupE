//Author: Drew Nguyen
/*
Responsibilities: Client implementation and Client-server architecture
        Reformatted handling of expressions and functions. Made code a little more clear and legible.
        Worked hard on trying to figure out certain bugs in terms of computer-client turns
        Drew worked along side adam to figure out how players would be added along side the computer player
*/

struct Player newPlayer(char *firstname, char *lastname, char *country)
{
	struct Player new_player;
	
	new_player.score = 0;
	strcpy(new_player.firstname, firstname);
	strcpy(new_player.lastname, lastname);
	strcpy(new_player.country, country);
	new_player.num_words = 0;
	new_player.num_words_added = 0;
    new_player.resets = 0;

	return new_player;
}

struct Computer newComputer()
{
	struct Computer new_computer;
	new_computer.score = 0;
	new_computer.num_words = 0;
	new_computer.num_words_added = 0;
	new_computer.resets = 0;

	return new_computer;
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
	}
	else
	{
		printf("MQ was opened \n");
	}
	return mqd;
}

void sendPlayerConnectMsg(mqd_t mqd)
{
	mq_send(mqd, "WAITING", 1, 10);
}

int recievePlayerConnectMsg(mqd_t mqd)
{
	int prio = 10;
    struct mq_attr attr;
	mq_getattr(mqd, &attr);
	char *p_buffer = calloc(attr.mq_msgsize, 1);
	int num_msgs = attr.mq_curmsgs;
	
	unsigned int priority = 0;
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

void sendDictionaryMsg(mqd_t mqd, char *message, int size)
{
	mq_send(mqd, message, size, 10);
}

char * recieveDictionaryMessage(mqd_t mqd)
{
    struct mq_attr attr;
    char *message = malloc(1024);
	mq_getattr(mqd, &attr);
	char *p_buffer = calloc(attr.mq_msgsize, 1);

    unsigned int priority = 0;
    if ((mq_receive(mqd, p_buffer, attr.mq_msgsize, &priority)) != -1)
    {
        // Collects message from queue
        if (priority == 10)
        {
            strcpy(message, p_buffer);
            return message;
        }
    }
}

void minusPlayerScore(struct Player new_player, int num)
{
    new_player.score += num;
}

void addPlayerScore(struct Player new_player)
{
    size_t length = nnew;
    if (length == 3 || length == 4)
    {
        new_player.score += 1;
    }
    if (length == 5)
    {
        new_player.score += 2;
    }
    if (length == 6)
    {
        new_player.score += 3;
    }
    if (length == 7)
    {
        new_player.score += 4;
    }
    if (length >= 8)
    {
        new_player.score += 11;
    }
}

void minusPlayerScore(struct Player new_player, int num)
{
    new_player.score += num;
}

void addPlayerScore(struct Player new_player)
{
    size_t length = nnew;
    if (length == 3 || length == 4)
    {
        new_player.score += 1;
    }
    if (length == 5)
    {
        new_player.score += 2;
    }
    if (length == 6)
    {
        new_player.score += 3;
    }
    if (length == 7)
    {
        new_player.score += 4;
    }
    if (length >= 8)
    {
        new_player.score += 11;
    }
}

void addComputerScore(struct Computer new_computer)
{
    size_t length = nnew;
    if (length == 3 || length == 4)
    {
        new_computer.score += 1;
    }
    if (length == 5)
    {
        new_computer.score += 2;
    }
    if (length == 6)
    {
        new_computer.score += 3;
    }
    if (length == 7)
    {
        new_computer.score += 4;
    }
    if (length >= 8)
    {
        new_computer.score += 11;
    }
}