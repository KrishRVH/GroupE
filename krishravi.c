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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

void computerTurn(char* prev, char** usedWords, char* letters, char* fname, int* noUsedWords)
{
    //Computer will check input file line by line for usable words (if they're wordbuilder words that haven't been used yet, it plays them)
    int prevlen = 0;
    char new[100];
    for (int i = 0; prev[i]!='\0'; i++)
    {
        prevlen++;
    }
    FILE* filePointer;
    int wordExist=0;
    int bufferLength = 99;
    char line[bufferLength];
    int linelen = 0;
    int disallowed = 0;
    int run = 1;
    char newf[101] = ""; //"new\n"  
    char newadd[101] = "\n";    // "\nnew\n"
    filePointer = fopen(fname, "r");
    while(fgets(line, bufferLength, filePointer) && run!=0)
    {
        strcpy(new,"");
        int c = getc(filePointer);
        if (c == EOF) 
        {
            printf("\nComputer could not find appropriate word");
            exit(0);
        }
        linelen=0;
        for (int i = 0; line[i]!='\0'; i++)
        {
            linelen++;
        }
        for(int i = 0; i < linelen; i++)
        {
            strcat(new[i],line[i]);
        }
        strcpy(newf,"");
        strcpy(newadd,"\n");
        strcat(newf, new);
        //strcat(newf,"\n");
        strcat(newadd, new);
        strcat(newadd,"\n");
        size_t n = sizeof(prev)/sizeof(char);
        size_t nnew = sizeof(new)/sizeof(char);
        size_t nnewf = sizeof(newf)/sizeof(char);
        for (int i=0; i<n;i++)
        {
            for (int x = 0; x < nnew && disallowed==0 && new[x]!='\0'; x++)
            {
                //make sure no disallowed characters are in it
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
                        //printf("\n Iteration %d we're looking at %c in new and %c in prev\n", i, prev[j], new[k]);
                        j++;
                        k++;
                        //printf("The value of j is %d k is %d n is %d", j,k,n);
                    }
                    if ((j==n) || (prev[j]=='\0') || ((new[k]=='\0') && (prev[j]=='\0')))
                    {
                        printf("\nComputer's Word is valid!");
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
                            //penalise
                            for (int i = 0; i<=noUsedWords;i++)
                            {
                                printf("\nUsed word %d of %d is %s",i,noUsedWords,usedWords[i]);
                            }
                            break;
                        }
                        else
                        {
                            printf("\nWord has NOT been used this game. Added to used words.");
                            for (int i = 0; i<=noUsedWords;i++)
                            {
                                printf("\nUsed word %d of %d is %s",i,noUsedWords,usedWords[i]);
                            }
                            run = 0;
                        }
                        strcpy(prev,new);
                        strcpy(new,"");
                        break;
                    }   
                    else
                    {
                        if (j<n)
                            continue;
                        printf("\n Invalid but part of it was at some point");
                        //in theory we should never be here?
                        //penalise
                        break;
                    }
                }
                if (i==(n-1))
                {
                    printf("\nComputer's Word is not valid.");
                    //penalise
                    break;
                }
            }
            else
            {
                printf("Word contains disallowed characters.");
                //penalise
                break;
            }
        }
    }
    fclose(filePointer);
}
void main()
{
    srand(time(NULL)); 
    int rng = (rand()%5)+1; //seeding random number from 1 to 10 for first turn word
    int rng2 = 1; //(rand()%10)+1; seeding random number from 1 to 10 for input.txt
    char rng2char[7];   
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
    printf("\nWe have chosen %s",fname);
    fileStream = fopen (fname, "r");
    fgets (letters, 7, fileStream); 
    fclose(fileStream);
    printf("\nLETTERS ARE: %s",letters);
    int disallowed = 0;
    // CODE TO CHECK VALIDITY OF NEW WORD AGAINST PREVIOUS WORD
    char prev[100]; //
    char new[100];
    char newf[101] = ""; //"new\n"  
    char newadd[101] = "\n";    // "\nnew\n"
    char usedWords[100][100];
    strcpy(usedWords[0],"COOKIE");
    strcpy(usedWords[1],"HEAVEC");
    int noUsedWords = 1;
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
    while (run!=0)
    {
        if (first == 1)
        {
            strcpy(new,prev);
            first = 0;
        }
        else
        {
            printf("\nEnter new word ");
            scanf("%99s",&new);
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
                        printf("\nConverting %s to lower",new);
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
                            printf("\nWord is a valid dictionary word!");
                        }
                        else 
                        {
                            printf("\nWord is not a valid dictionary word.");
                            //penalise
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
                            //penalise
                            for (int i = 0; i<=noUsedWords;i++)
                            {
                                printf("\nUsed word %d of %d is %s",i,noUsedWords,usedWords[i]);
                            }
                        }
                        else
                        {
                            printf("\nWord has NOT been used this game. Added to used words.");
                            for (int i = 0; i<=noUsedWords;i++)
                            {
                                printf("\nUsed word %d of %d is %s",i,noUsedWords,usedWords[i]);
                            }
                        }
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
                                break;
                            }
                        }
                        fclose(filePointer);
                        if (wordExist==1)
                        {
                            printf("\nWord was already in file.");
                        }
                        else 
                        {
                            printf("\nWord doesn't exist in the file and will be added.");
                            //add word to input file https://stackoverflow.com/questions/19429138/append-to-the-end-of-a-file-in-c
                            FILE * fptr;	
                            fptr = fopen(fname, "a"); 
                            fputs(newadd, fptr);
                            fclose (fptr);
                        }
                        strcpy(prev,new);
                        strcpy(new,"");
                        printf("\n TYPE 0 EXIT, ANY OTHER NUMBER FOR COMPUTER TURN");
                        scanf("%d",&run);                        
                        if (run==0)
                            exit(0);
                        else
                        {
                            //send everything to computer here, ensuring to update prev and new
                            computerTurn(&prev,&usedWords,&letters,&fname, &noUsedWords);
                            break;
                        }
                    }   
                    else
                    {
                        if (j<n)
                            continue;
                        printf("\n Invalid but part of it was at some point");
                        //in theory we should never be here?
                        //penalise
                        exit(0);
                    }
                }
                if (i==(n-1))
                {
                    printf("\n Word is not valid.");
                    //penalise
                    exit(0);
                }
            }
            else
            {
                printf("Word contains disallowed characters.");
                //penalise
                exit(0);
            }
        }
    }
}