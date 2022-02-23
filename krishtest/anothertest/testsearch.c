#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
void main()
{     
	FILE* filePointer;
	int wordExist=0;
	int bufferLength = 255;
	char search[100];
	printf("Enter word to be search=");
	scanf("%s",search);
	char line[bufferLength];
	filePointer = fopen("input_01.txt", "r");
	while(fgets(line, bufferLength, filePointer))
	{
		char *ptr = strstr(line, search);
		if (ptr != NULL) 
		{
			wordExist=1;
			break;
		}
	}
	fclose(filePointer);
	if (wordExist==1)
	{
		printf("Word exists.");
	}
	else 
	{
		printf("Word doesn't exist.");
	}
}  