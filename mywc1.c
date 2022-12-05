
/*  Name: Aimee Haas
    Program 6
    4-2022
    Program Name: mywc1.c
    Purpose: Basic word count program */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>

void readFile2Buf();
void countWords();
void oops(char*, char*);

char* readBuf;
int fileIndex, fileSize, origFd, bufFd, readBufIndex;
int perFileCount, totalCount;

int main(int argc, char* argv[])
{
    if(argc < 2)
        oops("Must provide file argument ", NULL);
    
    for(fileIndex = 1; fileIndex < argc; fileIndex++)
    {
        origFd = open(argv[fileIndex], O_RDONLY);
        if(origFd < 0)
            oops("File could not open.", argv[fileIndex]);
        readFile2Buf();

        //Reset before counting next file
        perFileCount = 0;
        countWords();
        printf("%7d %s\n", perFileCount, argv[fileIndex]);

        //Accumulates total when there is more than one file
        if(argc > 2)
            totalCount += perFileCount;
    }

    //Skips printing if only one file
    if(totalCount > 0)
        printf("%7d total\n", totalCount);

    free(readBuf);
}

void readFile2Buf()
{
    fileSize = lseek(origFd, 0, SEEK_END);
    lseek(origFd, 0, SEEK_SET);
    readBuf = (char*)malloc(fileSize);
    bufFd = read(origFd, readBuf, fileSize);
    if (bufFd < 0)
    {
        free(readBuf);
        oops("File could not copy to buffer.", NULL);
    }
    if (close(origFd) < 0)
        oops("Could not close orginial file.", NULL);
}

void countWords()
{
    readBufIndex = 0;
    while(readBuf[readBufIndex] != '\0')
    {
        if(isspace(readBuf[readBufIndex++]))
        {   
            //Avoids counting leading space as end of word
            if(readBufIndex > 1)
                perFileCount++;

            //Passes multiple space chars without adding to count
            while(isspace(readBuf[readBufIndex]))
                readBufIndex++; 
        }
    }
    //Handles case of last word not ending in a space character.
    if(!isspace(readBuf[readBufIndex-1]))
        perFileCount++;
        
}

void oops(char *s1, char *s2)
{
    fprintf(stderr, "Error: %s", s1);
    perror(s2);
    exit(1);
}