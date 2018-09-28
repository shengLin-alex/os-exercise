#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h> // file descriptor
#include <unistd.h> // unix standard

// define a content buffer size.
#define BUFFER_SIZE 8192

// Check file is exist or not.
int isFileExist(const char *filename)
{
    struct stat buf;
    if (stat(filename, &buf) != -1)
    {
        return 1;
    }

    return -1;
}

int main(int argc, char **argv)
{
    char* inputFileName = (char*)calloc(30, sizeof(char));
    char* outputFileName = (char*)calloc(30, sizeof(char));

    printf("Input source file name or path\n");
    scanf("%s", inputFileName);
    printf("Input destination file name or path\n");
    scanf("%s", outputFileName);

    if (isFileExist(inputFileName) == -1)
    {
        perror("Cannot find source file.\n");

        return 1;
    }

    // open target file, with readonly mode
    int inputFileDescriptor = open(inputFileName, O_RDONLY);
    if (inputFileDescriptor == -1)
    {
        perror("Cannot open source file.\n");

        return 1;
    }

    // open output file, with write only and create mode
    // 0(skip)6(User)4(Group)4(Other)means:
    // rwx oct    meaning
    // --- ---    -------
    // 001 01   = execute
    // 010 02   = write
    // 011 03   = write & execute
    // 100 04   = read
    // 101 05   = read & execute
    // 110 06   = read & write
    // 111 07   = read & write & execute
    // User: read & write
    // Group: read
    // Other: read
    // also can be written with "-rw-r--r--".
    int outputFileDescriptor = open(outputFileName, O_WRONLY | O_CREAT, 0644);
    if (outputFileDescriptor == -1)
    {
        perror("Cannot open output file.\n");

        return 1;
    }

    // the bytes size return by read() and write(). use signed size_t.
    ssize_t inputBytes, outputBytes;
    char* buffer = (char*)calloc(BUFFER_SIZE, sizeof(char));

    // write content until read to end.
    while((inputBytes = read(inputFileDescriptor, buffer, BUFFER_SIZE)) > 0) {
        outputBytes = write(outputFileDescriptor, buffer, inputBytes);

        if (inputBytes != outputBytes) {
            perror("I/O size not match.");

            return 1;
        }
    }

    printf("Content copy success.\n");

    close(inputFileDescriptor);
    close(outputFileDescriptor);

    free(inputFileName);
    free(outputFileName);
    free(buffer);

    return 0;
}