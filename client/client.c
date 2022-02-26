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


void playersTurn(){
    //receive from server:
    //          set of random alphabets
    //          player's current score
    //          opponents' current score
    //          words that have been used in the game so far
    //          starting character of the word that the player needs to form the word with
    
    //recv(clientSocket, buffer, 1024, 0);
    //strcpy(/*set of alphabets var*/, buffer);
    //bzero(buffer, sizeof(buffer));

    //recv(clientSocket, buffer, 1024, 0);
    //strcpy(/*player.score*/, buffer);
    //bzero(buffer, sizeof(buffer));

    //recv(clientSocket, buffer, 1024, 0);
    //strcpy(/*opponent's.score*/, buffer);
    //bzero(buffer, sizeof(buffer));

    //recv(clientSocket, buffer, 1024, 0);
    //strcpy(/*words already used*/, buffer);
    //bzero(buffer, sizeof(buffer));

    //recv(clientSocket, buffer, 1024, 0);
    //strcpy(/*Starting Character to form words */, buffer);
    //bzero(buffer, sizeof(buffer));    

    //printf("Set of random alphabets: %s\n", /*set of alphabets var*/);
    //printf("Your score is: %d\n", /*player score var*/);
    //printf("Your opponents score(s) (are/is): %d\n", /*still figuring out*/);
    //printf("Words that have already been used: %s\n", /*still figuring out*/);
    //printf("Starting character to form words: %s\n", /*still in process*/)

    //timer for 4minutes here or on server side?

    //printf("\nEnter your word: ");
    //scanf("%s", buffer[0]);
    //send(clientSocket, buffer, strlen(buffer), 0);

    //if server accepts word as valid
    //recv(clientSocket, buffer, 1024, 0);
    //strcpy(/*player.score*/, buffer);
    //bzero(buffer, sizeof(buffer));

    //recv(clientSocket, buffer, 1024, 0);
    //strcpy(/*all opponent's scores*/, buffer);
    //bzero(buffer, sizeof(buffer));
    
    //printf("\nYour updated score is: %d\n", /*player score var*/);
    //printf("Your opponent's score(s) (is/are): %d", /*still in progr*/);


    //questions: should clientSocket var be something else?
    //am i using recv, strcpy, and bzero correctly?
    //what to do about set of alphabets var, obtaining all opponent's scores
    //what to do about words already used, starting char to form words
    //do i need a struct on client side
    
}


// https://github.com/nikhilroxtomar/Multiple-Client-Server-Program-in-C-using-fork
int main()
{
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0)
	{
		printf("ERROR: Cannot create client socket.\n");
		exit(1);
	}
	printf("CONSOLE: Created client socket.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		printf("ERROR: Cannot connect to server.\n");
		exit(1);
	}
	printf("CONSOLE: Connected to Server.\n");

	while(1)
	{
		printf("Welcome to the word game.\n");
        printf("Please choose an option below (1 for Singleplayer, 2 for Multiplayer, 3 for exit)\n");
        printf("1) Singleplayer\n");
        printf("2) Multiplayer\n");
        printf("3) Exit\n");
        printf("> ");

		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);
        printf("Input: %s", buffer);

        if(strcmp(buffer, ":exit") == 0)
		{
            printf("\nSingle Player Mode\n");
            printf("Enter your first name: ");
            scanf("%s", &buffer[0]);
            send(clientSocket, buffer, strlen(buffer), 0);

            printf("\nEnter your last name: ");
            scanf("%s", &buffer[0]);
            send(clientSocket, buffer, strlen(buffer), 0);
            
            printf("\nEnter your country: ");
            scanf("%s", &buffer[0]);
            send(clientSocket, buffer, strlen(buffer), 0);

			// Game starts...

            int game_start = 1;
            if(game_start)
            {
                char test[50];
                recv(clientSocket, buffer, 1024, 0);
                strcpy(test, buffer);
                printf("Turn: %s\n", test);
                
                /* if (strcmp(buffer, "turn") == 0)
                {
                    // Players turn, recieves letters, list of words, etc.
                    char *letters;
                    bzero(buffer, sizeof(buffer));
                    recv(clientSocket, buffer, 1024, 0);
                    printf("Letters: %s\n", buffer);
                    strcpy(letters, buffer);

                    // Recieves number of list of words, then loops recv for words storing into array
                    int noUsedWords = 0;
                    char usedWords[100][100];

                    recv(clientSocket, buffer, 1024, 0);
                    noUsedWords = buffer[0] - '0';
                    printf("Number of used words: %i\n", noUsedWords);


                }
                else
                {
                    // Idk
                } */
            }

        }

        if(strcmp(buffer, "2") == 0)
		{
			//multiplayer
			//send connection to server
			//server will check to see if there is another client in multiplayer for 2mins
			//asks client to continue waiting if there is no one playing or play singleplayer
			//POSIX QUEUE MESSAGE FOR CLIENT CURRENTLY WAITING FOR ANOTHER PLAYER
			//CLIENT receives word when game starts
			//client sends word to call word check METHOD
			//work check METHOD returns a correct or not correct response
		}

		if(strcmp(buffer, "3") == 0)
		{
			close(clientSocket);
			printf("Exiting game. Closing application.\n");
			exit(1);
		}

        recv(clientSocket, buffer, 1024, 0);
        printf("Server: \t%s\n", buffer);
	}
	return 0;
}