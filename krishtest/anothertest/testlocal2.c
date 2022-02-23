#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
void main()
{
    srand(time(NULL)); 
    int rng = (rand()%9)+1; //seeding random number from 1 to 10 for first turn
    int rng2 = 3; //seeding random number from 1 to 10 for input.txt
    char rng2char[2];   
    sprintf(rng2char, "%d.txt", rng2);
    FILE *fileStream; 
    char letters [7];
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
    printf("\n%s",letters);
    fclose(fileStream);
    // CODE TO CHECK VALIDITY OF NEW WORD AGAINST PREVIOUS WORD
    char prev[6] = "DYE";
    char new[6] = "DYEB";
    int disallowed = 0;
    size_t n = sizeof(prev)/sizeof(char);
    size_t nnew = sizeof(new)/sizeof(char);
    for (int i=0; i<n;i++)
    {
        for (int x = 0; x < nnew && disallowed==0; x++)
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
            printf("\nUsed correct characters!");
            exit(0);
        }
        else
        {
            printf("\nUses illegal characters.");
            exit(0);
        }
    }
}