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
    char letters [100];
    char fname[12] = "";
    printf("\nrng2 generated was %d",rng2);
    if (rng2==10)
        strcat(fname, "input_");
    else    
        strcat(fname, "input_0");
    strcat(fname, rng2char);
    printf("\nWe have chosen %s",fname);
    fileStream = fopen (fname, "r");
    fgets (letters, 100, fileStream); 
    printf("\n%s",letters);
    fclose(fileStream);
    // CODE TO CHECK VALIDITY OF NEW WORD AGAINST PREVIOUS WORD
    char prev[6]; //word can never be longer than the 6 available chars
    char new[6];
    printf("\nEnter previous word ");
    gets(prev);
    printf("\nEnter new word ");
    gets(new);
    size_t n = sizeof(prev)/sizeof(char);
    for (int i=0; i<n;i++)
    {
        if (new[0]==prev[i])
        {
            int j = i;
            int k = 0;
            while ((j<n) && (new[k]==prev[j]) && (prev[j]!='\0') && (new[k]!='\0'))
            {
                printf("\nIteration %d we're looking at %c in new and %c in prev", i, prev[j], new[k]);
                j++;
                k++;
            }
            if (j==n)
            {
                printf("\nWord is valid!");
                exit(0);
            }   
            else
            {
                if (j<n)
                    continue;
                printf("\nlol it's invalid loser");
                exit(0);
            }
        }
    }
}