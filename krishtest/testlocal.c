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