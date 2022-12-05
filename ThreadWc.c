/*  Name: Aimee Haas
    4-2022
    Purpose: Handle multiple threads */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

void oops(char*, char*);

int fileIndex;
int totalFiles, totalCount;

int main(int argc, char* argv[])
{
    pthread_t thread[argc-1];
    void *process(void*);
    int *count[argc-1];
    if(argc < 2)
        oops("Must provide file argument ", NULL);
    
    totalFiles = argc-1;
    totalCount = 0;
    
    for(fileIndex = 0; fileIndex < totalFiles; fileIndex++)
        pthread_create(&thread[fileIndex], NULL, process, (void *)argv[fileIndex+1]);

    for(fileIndex = 0; fileIndex < totalFiles; fileIndex++)
        pthread_join(thread[fileIndex], (void**)&count[fileIndex]);
    
    for(fileIndex = 0; fileIndex < totalFiles; fileIndex++)
        totalCount += (*count[fileIndex]);
    
    for(fileIndex = 0; fileIndex < totalFiles; fileIndex++)
        printf("%7d %s\n", (*count[fileIndex]), argv[fileIndex+1]);

    //Skips total & printing if only one file
    if(totalCount > 0)
        printf("%7d total\n", totalCount);

}

void *process(void *f)
{       
    char *fileName = (char *)f;
    int origFd = open(fileName, O_RDONLY);
    if(origFd < 0)
        oops("File could not open.", fileName);

    //  Read file to buffer
    int fileSize = lseek(origFd, 0, SEEK_END);
    lseek(origFd, 0, SEEK_SET);
    char* readBuf = (char*)malloc(fileSize);
    int bufFd = read(origFd, readBuf, fileSize);
    if (bufFd < 0)
    {
        free(readBuf);
        oops("File could not copy to buffer.", NULL);
    }
    if (close(origFd) < 0)
        oops("Could not close orginial file.", NULL);

    int *perFileCount = malloc(sizeof(int));
    *perFileCount = 0;
    int readBufIndex = 0;

    while(readBuf[readBufIndex] != '\0')
    {
        if(isspace(readBuf[readBufIndex++]))
        {   
            //Avoids counting leading space as end of word
            if(readBufIndex > 1)
                (*perFileCount)++;
            //Passes multiple space chars without adding to count
            while(isspace(readBuf[readBufIndex]))
                readBufIndex++; 
        }
    }
    //Handles case of last word not ending in a space character.
    if(!isspace(readBuf[readBufIndex-1]))
        (*perFileCount)++;
    free(readBuf);
    return perFileCount;

}


void oops(char *s1, char *s2)
{
    fprintf(stderr, "Error: %s", s1);
    perror(s2);
    exit(1);
}
