#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
void main()
{
    srand(time(NULL)); 
    int rng = (rand()%9)+1; //seeding random number from 1 to 10 for first turn
    int rng2 = (rand()%9)+1; //seeding random number from 1 to 10 for input.txt
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
    fgets (letters, 6, fileStream); 
    printf("\n%s",letters);
    fclose(fileStream);
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
        for (int x = 0; x < nnew, !disallowed; x++)
        {
            for (int y = 0; y < 6; y++)
            {
                if (new[x]!=letters[y])
                    y++;
                if (y==6)
                    disallowed=1;
            }
        }
        if (!disallowed)
        {
            if (new[0]==prev[i])
            {
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
                        printf("Word was already in file and NOT been used.");
                        //add word to input.txt
                    }
                    else 
                    {
                        printf("Word doesn't exist.");
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
            printf("Word contains disallowed characters.")
        }
    }
}