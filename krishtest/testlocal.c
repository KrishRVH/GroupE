#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

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
                        printf("\n Iteration %d we're looking at %c in new and %c in prev\n", i, prev[j], new[k]);
                        j++;
                        k++;
                        //printf("The value of j is %d k is %d n is %d", j,k,n);
                    }
                    if ((j==n) || (prev[j]=='\0') || ((new[k]=='\0') && (prev[j]=='\0')))
                    {
                        printf("\n Word is valid!");
                        //check if word is a dictionary word
                        printf("\nConverting %s to lower",new);
                        for(int w = 0; w<nnewf; w++)
                        {
                            lowernew[w] = tolower(newf[w]);
                            printf("\nvalue %d of lowernew is %c",w,lowernew[w]);
                        }
                        FILE* filePointerd;
                        int wordExistd=0;
                        int bufferLengthd = 255;
                        char lined[bufferLengthd];
                        printf("\nChecking if %s is a valid dictionary word",lowernew);
                        filePointerd = fopen("dictionary.txt", "r");
                        while(fgets(lined, bufferLengthd, filePointerd))
                        {
                            char *ptrd = strstr(lined, lowernew);
                            if (ptrd != NULL) 
                            {
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
                        filePointer = fopen(fname, "r");
                        while(fgets(line, bufferLength, filePointer))
                        {
                            char *ptr = strstr(line, newf); //check newf in debugger
                            if (ptr != NULL) 
                            {
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
                        printf("\n TYPE 0 EXIT, ANY OTHER NUMBER TO CONTINUE. ");
                        scanf("%d",&run);
                        if (run==0)
                            exit(0);
                        else
                            break;
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