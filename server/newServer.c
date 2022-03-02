// Generic includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h> 
#include <sys/wait.h>

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

struct Player added_player;
struct Computer added_computer
char prev[100];
char new[100];
char newf[101] = ""; 
char newadd[101] = "\n";
char usedWords[100][100];
int noUsedWords = 1;
char letters [6];
char fname[14] = "";

#define MAX 50
#define PORT 8000
#define SA struct sockaddr

// Player struct
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

void playerTurn(newSocket)
{
    srand(time(NULL)); 
    int rng = (rand()%5)+1; //seeding random number from 1 to 10 for first turn word
    int rng2 = 1; //(rand()%10)+1; seeding random number from 1 to 10 for input.txt
    char rng2char[7];   
    sprintf(rng2char, "%d.txt", rng2);
    FILE *fileStream; 
    printf("\nrng generated was %d",rng);
    if (rng2==10)
        strcat(fname, "input_");
    else    
        strcat(fname, "input_0");
    strcat(fname, rng2char);
    printf("\nWe have chosen %s",fname);
    fileStream = fopen (fname, "r");
    fgets (letters, 7, fileStream); 
    fclose(fileStream);

    // FIRST TURN

    // Socket variables
	char buffer[1024];

    // Sends letters to client
    bzero(buffer, sizeof(buffer));
    strcpy(buffer, letters);
    send(newSocket, buffer, 1024, 0);

    for (int i = 0; i<=noUsedWords;i++)
    {
        printf("\nUsed word %d of %d is %s",i,noUsedWords,usedWords[i]);
    }
    int run = 1;
    int first = 1;
    printf("\nFirst turn! Enter a valid word starting with the letter %c ",letters[rng]);
    gets(prev);
    if (prev[0]!=letters[rng])
    {
        printf("\n%c is not %c Invalid starting character.",prev[0],letters[rng]);
        //penalise
        exit(0);
    }
    
    int disallowed = 0;


}

int createServer()
{
    int sockfd, ret, newSocket;
	struct sockaddr_in serverAddr, newAddr;
	socklen_t addr_size;
	char buffer[1024];
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
    if(listen(sockfd, 10) == 0){
		printf("[+]Listening..\n..\n\n");
	} else {
		printf("[-]Error in binding.\n");
	}

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
				printf("%s\n", buffer);
                if(true) //strcmp(buffer, "1")) just gonna default to singleplayer for now...
				{
					// Single player game

                    // Receiving player information 
                    printf("getting first name..\n");
                    recv(newSocket, buffer, 1024, 0);
                    strcpy(firstname, buffer);
                    bzero(buffer, sizeof(buffer));
                    
                    printf("getting last name..\n");
                    recv(newSocket, buffer, 1024, 0);
                    strcpy(lastname, buffer);
                    bzero(buffer, sizeof(buffer));

                    printf("getting country..\n");
                    recv(newSocket, buffer, 1024, 0);
                    strcpy(country, buffer);
                    bzero(buffer, sizeof(buffer));

                    // Create new player and computer struct
                    added_player = newPlayer(firstname, lastname, country);
					added_computer = newComputer();

					printf("First: %s Last: %s Country: %s", added_player.firstname, 
					added_player.lastname, added_player.country);

					bzero(firstname, sizeof(firstname));
					bzero(lastname, sizeof(lastname));
					bzero(country, sizeof(country));

					// Game starts..\n.
					int game_start = 1;
					if(game_start)
					{
                        playerTurn(newSocket);

					}
				}
                /*else if(strcmp(buffer, "2"))
				{
					// Multiplayer game
                    if (DEBUGGER)
                        printf("multiplayer\n");
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
				else if(strcmp(buffer, "3") == 0)
				{
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}*/ // this is broke
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
    createServer();
    return 0;
}