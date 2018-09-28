#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(int argc, char** argv)
{
    char writeMessage[BUFFER_SIZE] = "Hello world!";
    char readMessage[BUFFER_SIZE];
    int fileDescriptor[2];
    pid_t pid;

    // create a pipe
    if (pipe(fileDescriptor) == -1)
    {
        fprintf(stderr, "Failed to create pipe.");

        return 1;
    }

    // fork a child process.
    pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Ford failed.");

        return 1;
    }

    // parent process for write
    if (pid > 0)
    {
        // must close unused end of the pipe.
        close(fileDescriptor[READ_END]);

        // write to the pipe
        // strlen(char*) + 1, "+1" because last char is '\0'
        write(fileDescriptor[WRITE_END], writeMessage, strlen(writeMessage) + 1);

        close(fileDescriptor[WRITE_END]);
    }
    else
    {
        // child process for read.
        close(fileDescriptor[WRITE_END]);

        read(fileDescriptor[READ_END], readMessage, BUFFER_SIZE);
        printf("read %s", readMessage);

        close(fileDescriptor[READ_END]);
    }

    return 0;
}