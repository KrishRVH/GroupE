// Main header for each source file.

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
#include <sys/wait.h>
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
mqd_t mqd;
struct mq_attr attr; // Only used for buffer size/declaration in mqueue
char* p_buffer;

// Player struct
struct Player
{
	char firstname[50];
	char lastname[50];
	char country[50];
	int score;
	int num_words;
	int num_words_added;
} Player;

int player();
void server();
void client();
void openMsgQueue();
void closeMsgQueue();
void recievePlayerMsg();