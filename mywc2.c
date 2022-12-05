/*  Name: Aimee Haas
    Program 6
    4-2022
    Program Name: mywc2.c
    Purpose: Handle multiple pipes for simutaneous processing */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

void add2Total();
void oops(char*, char*);

int pid, pipeIndex, fileIndex, totalCount;
char outputBuf[100];

int main(int argc, char* argv[])
{   
    if(argc < 2)
        oops("Must provide file argument.", NULL);

    int pipefd[argc - 1][2];
    
    //  Sets up the stdin pipe for every child process
    //  execs each process to run simutanously 
    for(fileIndex = 1; fileIndex < argc; fileIndex++)
    {
        if(pipe(pipefd[fileIndex]) == -1)
            oops("Pipe Problems.", NULL);
    
        if ((pid = fork()) == -1)
            oops("Fork Problems.", NULL);
        
        if(pid == 0)
        {
            if (dup2(pipefd[fileIndex][1], 1) == -1)
                oops("Dup stdin problems.", NULL);
            execl("mywc1", "mywc1", argv[fileIndex], NULL);
            perror("execvp failed"); 
            exit(1);
        }
    }
    
    //  Collects stdout from every pipe 
    //  Tokenizes each perfile total to add to overall total.
    for(fileIndex = 1; fileIndex < argc; fileIndex++)
    {
        if(dup2(pipefd[fileIndex][0], 0) == -1)
            oops("Dup stdout problems.", NULL);
        int nextString = read(0, &outputBuf, 100);
        if (nextString == -1)
            oops("Could not read to buffer", NULL);
        outputBuf[nextString] = '\0';
        printf("%s", outputBuf);
        add2Total();
        close(pipefd[fileIndex][0]);
        close(pipefd[fileIndex][1]);
    }

    if(totalCount > 0)
        printf("%7d total\n", totalCount);
}

void add2Total()
{
    char * token = strtok(outputBuf, " ");
    totalCount += atoi(token);
}

void oops(char *s1, char *s2)
{
    fprintf(stderr, "Error: %s", s1);
    perror(s2);
    exit(1);
}
