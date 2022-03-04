// Main header for each source file.

// Generic includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h> 
#include <sys/wait.h>
#include <time.h>
#include <stdint.h>

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

// RNG Seeding
#include <time.h>

// Constants
#define MAX 50
#define PORT 8000
#define SA struct sockaddr

// Globals
struct Player added_player;
struct Computer added_computer;
char prev[100];
char new[100];
char newf[101] = ""; 
char newadd[101] = "\n";
char usedWords[100][100];
uint32_t noUsedWords = 1;
char letters [6];
char fname[14] = "";
size_t nnew;

// Adam Loeckle
struct Player newPlayer(char *firstname, char *lastname, char *country);
struct Computer newComputer();
mqd_t openMsgQueue(char *queue_name);
void closeMsgQueue(mqd_t mqd);
void sendPlayerConnectMsg(mqd_t mqd);
int recievePlayerConnectMsg(mqd_t mqd);
void sendDictionaryMsg(mqd_t mqd, char *message, int size);
char * recieveDictionaryMessage(mqd_t mqd);
int playerTurn(int newSocket);
int createServer();
int clientGame();
struct Player
{
	int score;
	char firstname[50];
	char lastname[50];
	char country[50];
	int num_words;
	int num_words_added;
	int resets;
} Player;
struct Computer
{
	int score;
	int num_words;
	int num_words_added;
	int resets;
} Computer;

// Drew
void minusPlayerScore(struct Player new_player, int num);
void addPlayerScore(struct Player new_player);
void addComputerScore(struct Computer new_computer);

// Krish
int computerTurn();
void dictionaryCheck(mqd_t dictionary, size_t nnewf, char *lowernew, int newSocket);
int inputCheck();
int gameLogic(int newSocket, char *buffer);

// IDK
int clientGame();