#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // file descriptor

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 2048

int main(int argc, char** argv)
{
    // check arguments number
    if (argc != 3)
    {
        fprintf(stderr, "Wrong argument number");

        return 1;
    }

    int* fileDescriptors = (int*)calloc(2, sizeof(int));
    char* parentBuffer = (char*)calloc(BUFFER_SIZE, sizeof(char));
    char* childBuffer = (char*)calloc(BUFFER_SIZE, sizeof(char));

    // take the program arguments.
    char* sourceFile = argv[1];
    char* destinationFile = argv[2];

    // create pipe
    if (pipe(fileDescriptors) == -1)
    {
        fprintf(stderr, "Failed to create pipe.");

        return 1;
    }

    // fork child process.
    pid_t pid = fork();
    if (pid < 0) // -1
    {
        fprintf(stderr, "Ford failed.");

        return 1;
    }

    // parent process for writing
    if (pid > 0)
    {
        // must close unused end of the pipe.
        close(fileDescriptors[READ_END]);

        // open source with read only
        int sourceFileDescriptor = open(sourceFile, O_RDONLY);
        if (sourceFileDescriptor == -1)
        {
            fprintf(stderr, "Cannot open source file.\n");

            return 1;
        }

        // get input byte numbers
        ssize_t inputBytes = read(sourceFileDescriptor, parentBuffer, BUFFER_SIZE);

        // write bytes to child process
        ssize_t outputBytes = write(fileDescriptors[WRITE_END], parentBuffer, inputBytes);

        if (inputBytes != outputBytes)
        {
            fprintf(stderr, "I/O size not match.");

            return 1;
        }

        close(fileDescriptors[WRITE_END]);
    }
    else // child process
    {
        // close write end of the pipe.
        close(fileDescriptors[WRITE_END]);

        // read content from read end.
        ssize_t outputBytes = read(fileDescriptors[READ_END], childBuffer, BUFFER_SIZE);

        close(fileDescriptors[READ_END]);

        // get output target file descriptor.
        // check 0644 mean in ../POSIX-file-api/main.c
        int destinationFileDescriptor = open(destinationFile, O_WRONLY | O_CREAT, 0644);
        if (destinationFileDescriptor == -1)
        {
            fprintf(stderr, "Cannot open output file.\n");

            return 1;
        }

        // write content to target file descriptor.
        ssize_t inputBytes = write(destinationFileDescriptor, childBuffer, outputBytes);

        if (inputBytes != outputBytes)
        {
            fprintf(stderr, "I/O size not match.");

            return 1;
        }
    }

    free(fileDescriptors);
    free(parentBuffer);
    free(childBuffer);

    return 0;
}