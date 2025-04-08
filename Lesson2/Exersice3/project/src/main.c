#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    const char *fileName = argv[1];
    int numBytes = atoi(argv[2]);
    char mode = argv[3][0];
    const char *data = argv[4];

    if (argc != 5) {
        fprintf(stderr, "Usage: %s filename num-bytes [r/w] \"data\"\n", argv[0]);
        return 1;
    }

    if(numBytes <= 0)
    {
        printf("num-bytes must be a positive integer\n");
        return 1;
    }

    if(mode == 'r')
    {
        int fd = open(fileName, O_RDONLY);
        if(fd == -1)
        {
            perror("open failed!!!");
            return 1;
        }

        char *buffer = malloc(numBytes + 1);
        if(!buffer)
        {
            perror("Buffer alloc faled");
            close(fd);
            return 1;
        }


        size_t bytesRead = read(fd, buffer, numBytes);
        if(bytesRead == -1)
        {
            perror("Read failed!!!");
            close(fd);
            return 1;
        }

        buffer[bytesRead] = '\0';
        printf("Data: %s\n", buffer);

        free(buffer);
        close(fd);
    }
    else if(mode = 'w')
    {
        int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if(fd == -1)
        {
            perror("open failed!!!");
            return 1;
        }
        size_t bytesToWrite = (numBytes < strlen(data)) ? numBytes : strlen(data);
        size_t bytesWrite = write(fd, data, bytesToWrite);
        if(bytesWrite == -1)
        {
            perror("Read failed!!!");
            close(fd);
            return 1;
        }

        printf("Wrote %ld bytes to %s", bytesWrite, fileName);
        close(fd);
    }
    else
    {
        printf("Invalid mode!!!Use 'r' or 'w'\n");
        return 1;
    }

    return 0;
}