// Generic includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

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

// Globals
int incorrect = 0;

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
	mq_getattr(mqd, &attr);
	p_buffer = calloc(attr.mq_msgsize, 1);
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

// Send message for game instruction
void sendGameMsg(mqd_t mqd, char *message, int size)
{
	mq_send(mqd, message, size, 10);
}

char* recieveMsg(mqd_t mqd)
{
    char message[1024];
	mq_getattr(mqd, &attr);
	p_buffer = calloc(attr.mq_msgsize, 1);

	printf("# Of messages: %ld\n", attr.mq_curmsgs);
	int num_msgs = attr.mq_curmsgs;
	
	unsigned int priority = 0;
	if (num_msgs != 0)
	{
		if ((mq_receive(mqd, p_buffer, attr.mq_msgsize, &priority)) != -1)
		{
			// Collects message from queue
			if (priority == 10)
			{
				printf("Message: %s, Prio: %i\n", p_buffer, priority);
                strcpy(message, p_buffer);
			}

            // TESTING PURPOSES
			// Player struct information message, recieves following from player:
			// - Set of random alphabets (Global var)
			// - Current player score (Local for client fork)
			// - Other players score ***
			// - Words that have been used so far (Global var)
			if (priority == 1)
			{
				struct Player* new_player = (struct Player*)p_buffer;
				printf("Player: %i, Prio: %i\n", new_player[0].score, priority);
			}
		}
		//num_msgs -= 1;
	}
	free(p_buffer);
    return message;
}

// Penalize player for incorrect.
void penalise(int pen, int newSocket, struct Player *player) {
    char buffer[1024];
    if (incorrect == 3) {
        bzero(buffer, sizeof(buffer));
        strcpy(buffer, "incorrect_fin");
        send(newSocket, buffer, 1024, 0);
        incorrect = 0;
        player->score -= pen;
	    player->resets += pen;
        return;
    }

    bzero(buffer, sizeof(buffer));
    strcpy(buffer, "incorrect");
    send(newSocket, buffer, 1024, 0);

    player->score -= pen;
	player->resets += pen;

    incorrect++;
}

void addPoints(int pen, struct Player *player)
{
    player.score += pen;
}

int dictionaryCheck(char *newf, size_t nnewf, char *lowernew, int newSocket)
{
    for(int w = 0; w<nnewf; w++)
    {
        lowernew[w] = tolower(newf[w]);
    }
    FILE* filePointerd;
    int wordExistd=0;
    int bufferLengthd = 255;
    char lined[bufferLengthd];
    int linedlen = 0;
    int lowernewlen = 0;
    printf("\nChecking if %s is a valid dictionary word",lowernew);
    filePointerd = fopen("dictionary.txt", "r");
    for (int i = 0; lowernew[i]!='\0'; i++)
    {
        lowernewlen++;
    }
    while(fgets(lined, bufferLengthd, filePointerd))
    {
        linedlen=0;
        for (int i = 0; lined[i]!='\0'; i++)
        {
            linedlen++;
        }
        char *ptrd = strstr(lined, lowernew);
        if (ptrd != NULL && (linedlen==lowernewlen)) 
        {
            //printf("\nline is %d characters long and newf is %d long",linedlen,lowernewlen);
            wordExistd=1;
            break;
        }
    }
    bzero(lowernew,sizeof(lowernew));
    fclose(filePointerd);
    if (wordExistd==1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int inputCheck()
{
    //check if word is already in the input file https://www.efaculty.in/c-programs/check-whether-a-given-word-exists-in-a-file-or-not-program-in-c/
    FILE* filePointer;
    int wordExist=0;
    int bufferLength = 255;
    char line[bufferLength];
    int linelen = 0;
    int newflen = 0;
    for (int i = 0; newf[i]!='\0'; i++)
    {
        newflen++;
    }
    filePointer = fopen(fname, "r");
    while(fgets(line, bufferLength, filePointer))
    {
        linelen=0;
        for (int i = 0; line[i]!='\0'; i++)
        {
            linelen++;
        }
        char *ptr = strstr(line, newf); //check newf in debugger
        if (ptr != NULL && (linelen==newflen)) 
        {
            //printf("\nINPUT.txt line is %d characters long and newf is %d long",linelen,newflen);
            wordExist=1;
            return 0;
        }
    }
    return 1;
    fclose(filePointer);
}


// Single player
void playerTurn(int newSocket, struct Player *player, struct Computer *computer, mqd_t dictionary_check, mqd_t input_check)
{
	// Socket variables
	char buffer[1024];

	// Correct/incorrect
	char answer[] = "incorrect";

    srand(time(NULL)); 
    int rng = (rand()%5)+1; //seeding random number from 1 to 10 for first turn word
    int rng2 = 1; //(rand()%10)+1; seeding random number from 1 to 10 for input.txt
    char rng2char[7];

	// File selection, gets random letters
    sprintf(rng2char, "%d.txt", rng2);
    FILE *fileStream; 
    char letters [6];
    char fname[14] = "";
    printf("\nrng generated was %d",rng);
    if (rng2==10)
        strcat(fname, "input_");
    else    
        strcat(fname, "input_0");
    strcat(fname, rng2char);
    fileStream = fopen (fname, "r");
    fgets (letters, 7, fileStream); 
    fclose(fileStream);
	
    int disallowed = 0;

    // CODE TO CHECK VALIDITY OF NEW WORD AGAINST PREVIOUS WORD
    char prev[100]; //
    char new[100];
    char newf[101] = ""; //"new\n"  
    char newadd[101] = "\n";    // "\nnew\n"
    int noUsedWords = 0;
    char usedWords[100][100];

	// Sends number of resets that have been used by the client
	char num_resets = player->resets + '0';
	bzero(buffer, sizeof(buffer));
	strcpy(buffer, &num_resets);
	send(newSocket, buffer, 1024, 0);

    // Sends starting letters
    bzero(buffer, sizeof(buffer));
    strcpy(buffer, letters);
    send(newSocket, buffer, 1024, 0);

	// ----------------------------------------------------------------------------------
    // Sends starting character to client
    //printf("\nFirst turn! Enter a valid word starting with the letter %c ",letters[rng]);
    bzero(buffer, sizeof(buffer));
    strcpy(buffer, &letters[rng]);
    send(newSocket, buffer, 1024, 0);
    
	int run = 1;
    int first = 1;
    while (run!=0)
    {
        // Sends number of used words and used words to client
        char num_used_words = noUsedWords + '0';
        bzero(buffer, sizeof(buffer));
        strcpy(buffer, &num_used_words);
        send(newSocket, buffer, 1024, 0);

        for (int i = 0; i <= noUsedWords; i++)
        {
            printf("\nUsed word %d of %d is %s",i,noUsedWords,usedWords[i]);
            bzero(buffer, sizeof(buffer));
            strcpy(buffer, usedWords[i]);
            send(newSocket, buffer, 1024, 0);
        }

        // Recieves first word from client
        //gets(prev);
        bzero(buffer, sizeof(buffer));
        recv(newSocket, buffer, 1024, 0);
        strcpy(prev, buffer);


        if (prev[0]!=letters[rng]) {
            printf("\n%c is not %c Invalid starting character.",prev[0],letters[rng]);
            penalise(1, newSocket, &player);
            if (incorrect == 3) { break; } else { continue; }
        }

        if (first == 1)
        {
            strcpy(new,prev);
            first = 0;
        }

        strcpy(newf,"");
        strcpy(newadd,"\n");
        strcat(newf, new);
        strcat(newf,"\n");
        strcat(newadd, newf);
        size_t n = sizeof(prev)/sizeof(char);
        size_t nnew = sizeof(new)/sizeof(char);
        size_t nnewf = sizeof(newf)/sizeof(char);
        char lowernew[101];
        for (int i=0; i<n;i++)
        {
            for (int x = 0; x < nnew && disallowed==0 && new[x]!='\0'; x++)
            {
                for (int y = 0; y < 6 && new[x]!='\0'; y++)
                {
                    //printf("\n Iteration %d y iteration %d we're looking at %c in new and %c in letters\n", x, y, new[x], letters[y]);
                    if (new[x]!=letters[y])
                    {
                        if (letters[y+1]=='\0')
                            disallowed=1;
                        else
                            continue;
                    }
                    else
                        break;
                }
            }
            if (disallowed==0)
            {
                if (new[0]==prev[i])
                {
                    printf("\nUsed correct characters!");
                    int j = i;
                    int k = 0;
                    while ((j<n) && (new[k]==prev[j]) && !((new[k]=='\0') && (prev[j]=='\0')))
                    {
                        //printf("\n Iteration %d we're looking at %c in new and %c in prev\n", i, prev[j], new[k]);
                        j++;
                        k++;
                        //printf("The value of j is %d k is %d n is %d", j,k,n);
                    }
                    if ((j==n) || (prev[j]=='\0') || ((new[k]=='\0') && (prev[j]=='\0')))
                    {
                        printf("\nWord is valid!");
                        //check if word is a dictionary word

                        if (fork() == 0)
                        {
                            int valid_dict = dictionaryCheck(newf, nnewf, lowernew, newSocket);
                            if (valid_dict == 1){}
                            else
                            {
                                char message[8] = "INVALID";
                                sendGameMsg(dictionary_check, message, 8);
                            }
                        }
                        else
                        {
                            wait(NULL);
                            char *valid_input = inputCheck(usedWords, noUsedWords);
                            if (valid_input){}
                            else
                            {
                                char message[8] = "INVALID";
                                sendGameMsg(input_check, message, 8);
                            }
                        }
                        
                        if (strcmp(recieveMsg(dictionary_check), "INVALID"))
                        {
                            penalise(1, newSocket, &player);
                            if (incorrect == 3) { break; } else { continue; }
                        }

                        if (strcmp(recieveMsg(input_check), "INVALID"))
                        {
                            addPoints(5, &player);
                            if (incorrect == 3) { break; } else { continue; }
                        }

                        //check if word has already been used https://stackoverflow.com/questions/63132911/check-if-a-string-is-included-in-an-array-and-append-if-not-c
                        int dup = 0;
                        for (int j = 0; j < 100; j++) 
                        {
                            if(strcmp(new, usedWords[j]) == 0) 
                            {
                                dup = 1;   // got a duplicate
                                break;    
                            }
                        }
                        if (dup == 0) {    // not a duplicate: add it to usedWords
                            strcpy(usedWords[noUsedWords+1], new);
                            noUsedWords += 1;
                        }
                        if(dup) 
                        {
                            printf("\nWORD HAS ALREADY BEEN USED THIS GAME.");
                            for (int i = 0; i<=noUsedWords;i++)
                            {
                                printf("\nUsed word %d of %d is %s",i,noUsedWords,usedWords[i]);
                            }
                            penalise(2, newSocket, &player);
                            if (incorrect == 3) { break; } else { continue; }
                        }
                        else
                        {
                            printf("\nWord has NOT been used this game. Added to used words.");
                            for (int i = 0; i<=noUsedWords;i++)
                            {
                                printf("\nUsed word %d of %d is %s",i,noUsedWords,usedWords[i]);
                            }
                        }

                        strcpy(prev,new);
                        strcpy(new,"");
                        //printf("\n TYPE 0 EXIT, ANY OTHER NUMBER TO CONTINUE. ");
                        //scanf("%d",&run);
                        if (run==0)
                            exit(0);
                        else
                            bzero(buffer, sizeof(buffer));
                            strcpy(buffer, "correct");
                            send(newSocket, buffer, 1024, 0);

                            char char_player_score = player->score + '0';
                            bzero(buffer, sizeof(buffer));
                            strcpy(buffer, &char_player_score);
                            send(newSocket, buffer, 1024, 0);

                            // if multiplayer, write a for loop that sends multiple scores depending on amt of players.
                            char char_computer_score = computer->score + '0';
                            bzero(buffer, sizeof(buffer));
                            strcpy(buffer, &char_computer_score);
                            send(newSocket, buffer, 1024, 0);
                            break;
                    }   
                    else
                    {
                        if (j<n)
                            continue;
                        printf("\n Invalid but part of it was at some point");
                        //in theory we should never be here?
                        penalise(1, newSocket, &player);
                        if (incorrect == 3) { break; } else { continue; }
                    }
                }
                if (i==(n-1))
                {
                    printf("\n Word is not valid.");
                    penalise(1, newSocket, &player);
                    if (incorrect == 3) { break; } else { continue; }
                }
            }
            else
            {
                printf("Word contains disallowed characters.");
                penalise(1, newSocket, &player);
                if (incorrect == 3) { break; } else { continue; }
            }
        }
    }
}

// Test server socket code for multiple forked clients
int main()
{
	mqd_t dictionary_check = openMsgQueue("/Dictionary_check");
    mqd_t input_check = openMsgQueue("/Input_check");

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
		printf("[+]Listening....\n");
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
                if(strcmp(buffer, "1") == 0)
				{
					// Single player game

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

                    // Create new player and computer struct
                    struct Player added_player = newPlayer(firstname, lastname, country);
					struct Computer added_computer = newComputer();

					printf("First: %s Last: %s Country: %s", added_player.firstname, 
					added_player.lastname, added_player.country);

					bzero(firstname, sizeof(firstname));
					bzero(lastname, sizeof(lastname));
					bzero(country, sizeof(country));

					// Game starts...
					int game_start = 1;
					if(game_start)
					{
						// turn input
						scanf("%s", &buffer[0]);
						send(newSocket, buffer, 1024, 0);

                        playerTurn(newSocket, &added_player, &added_computer, dictionary_check, input_check);

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