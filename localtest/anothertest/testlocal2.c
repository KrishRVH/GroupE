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
    printf("\n rng2 generated was %d",rng2);
    if (rng2==10)
        strcat(fname, "input_");
    else    
        strcat(fname, "input_0");
    strcat(fname, rng2char);
    printf("\n We have chosen %s",fname);
    fileStream = fopen (fname, "r");
    fgets (letters, 100, fileStream); 
    printf("\n%s",letters);
    fclose(fileStream);
    // CODE TO CHECK VALIDITY OF NEW WORD AGAINST PREVIOUS WORD
    char prev[6] = "aellae";
    char new[6] = "ae";
    size_t n = sizeof(prev)/sizeof(char);
    for (int i=0; i<n;i++)
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
                exit(0);
            }   
            else
            {
                if (j<n)
                    continue;
                printf("\n lol it's invalid loser");
                exit(0);
            }
        }
        if (i==(n-1))
        {
            printf("\n lol it's invalid loser + ratio");
            exit(0);
        }
    }
}