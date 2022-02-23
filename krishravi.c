//Author: Krish Ravi
/*
Scoreboard logic + Text file from Assignment00

Match starting letter of prospective word to previous word, and then consecutively until end of previous word to check if eligible word
Starting player is random (use random number gen)
if both players pass, new alphabet
if both players pass twice in a row (keep back and forth counter) the game ends

pseudocode to check validity of new word (new)
Precondition: prev is a character array of previous valid word, new is character array prospective new word
for (int i=0; i<sizeof prev;i++)
{
    if (new[0]==prev[i])
    {
        int j = i;
        int k = 0;
        while ((prev[j]!=null) && (new[k]==prev[j])
        {
            j++;
            k++;
        }
        if (j==sizeof prev)
         word is valid
    }
}

Flow of game:
Randomly select input.txt and player1
Display first line to player1, select 1 character and require prev = just that character
word is sent to server, if valid, score updates, player1 gets both player scores from server, then player2 is shown the line from input.txt and so on

During player turn, server sends:
-> input.txt line 1
-> player score
-> other player score
-> Used words
-> prev requirements

During player sending word, server checks
-> received word validity
-> whether word is used
-> assign score as per input.txt (should be based on length)
if invalid
-> penalise for invalid word, request valid entry
if used
-> inform used, penalise
each player gets 4 minutes to enter a word
timer is reset when a word is sent to the server
if player resets the timer 3 times in one turn server considers it a pass
when a new valid word is found, it needs to be added to input.txt and scored accordingly
in singleplayer the server is only allowed to use input.txt words, not dictionary.txt
*/

// Game logic, handles recieving/sending messages
void game()
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
void main()
{
    srand(time(NULL)); 
    int rng = (rand()%9)+1; //seeding random number from 1 to 10 for first turn
    int rng2 = 1; //(rand()%9)+1; seeding random number from 1 to 10 for input.txt FOR SOME REASON IT WON'T GENERATE 10
    char rng2char[2];   
    sprintf(rng2char, "%d.txt", rng2);
    FILE *fileStream; 
    char letters [6];
    char fname[12] = "";
    printf("\nrng2 generated was %d",rng2);
    if (rng2==10)
        strcat(fname, "input_");
    else    
        strcat(fname, "input_0");
    strcat(fname, rng2char);
    printf("\nWe have chosen %s",fname);
    fileStream = fopen (fname, "r");
    fgets (letters, 7, fileStream); 
    fclose(fileStream);
    printf("\nLETTERS ARE: %s",letters);
    int disallowed = 0;
    // CODE TO CHECK VALIDITY OF NEW WORD AGAINST PREVIOUS WORD
    char prev[6]; //word can never be longer than the 6 available chars
    char new[6];
    printf("\nEnter previous word ");
    gets(prev);
    printf("\nEnter new word ");
    gets(new);
    size_t n = sizeof(prev)/sizeof(char);
    size_t nnew = sizeof(new)/sizeof(char);
    for (int i=0; i<n;i++)
    {
        for (int x = 0; x < nnew && disallowed==0 && new[x]!='\0'; x++)
        {
            for (int y = 0; y < 6 && new[x]!='\0'; y++)
            {
                printf("\n Iteration %d y iteration %d we're looking at %c in new and %c in letters\n", x, y, new[x], letters[y]);
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
                    printf("\n Iteration %d we're looking at %c in new and %c in prev\n", i, prev[j], new[k]);
                    j++;
                    k++;
                    printf("The value of j is %d k is %d n is %d", j,k,n);
                }
                if ((j==n) || (prev[j]=='\0') || ((new[k]=='\0') && (prev[j]=='\0')))
                {
                    printf("\n Word is valid!");
                    //check if word has already been used this game https://www.efaculty.in/c-programs/check-whether-a-given-word-exists-in-a-file-or-not-program-in-c/
                    FILE* filePointer;
                    int wordExist=0;
                    int bufferLength = 255;
                    char line[bufferLength];
                    printf("\nFilename is %s",fname);
                    filePointer = fopen(fname, "r");
                    while(fgets(line, bufferLength, filePointer))
                    {
                        char *ptr = strstr(line, new);
                        if (ptr != NULL) 
                        {
                            wordExist=1;
                            break;
                        }
                    }
                    fclose(filePointer);
                    if (wordExist==1)
                    {
                        printf("\nWord was already in file and NOT been used.");
                        //add word to input.txt
                    }
                    else 
                    {
                        printf("\nWord doesn't exist in the file.");
                    }
                    exit(0);
                }   
                else
                {
                    if (j<n)
                        continue;
                    printf("\n Invalid but part of it was at some point");
                    exit(0);
                }
            }
            if (i==(n-1))
            {
                printf("\n Word is not valid.");
                exit(0);
            }
        }
        else
        {
            printf("Word contains disallowed characters.");
            exit(0);
        }
    }
}