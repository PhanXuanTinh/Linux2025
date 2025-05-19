#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// Do mở file, cờ O_APPEND được bật nên dù có lseek trỏ vể đầu file 
// thì dữ liệu vẫn được ghi vào cuối file.

int main()
{
    //open file, contain flag O_APPEND
    int fd = open("test.txt", O_APPEND | O_RDONLY | O_WRONLY | O_CREAT);
    if(fd == -1)
    {
        perror("File can't open!!!");
        return 1;
    }

    //Seek to begin-point
    if(lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("lseek error!!!");
        return 1;
    }

    //write data
    const char* data = "This is message which sent from Tinh.Phan!!!\n";
    size_t byteWritten = write(fd, data, strlen(data));
    if(byteWritten == -1)
    {
        perror("Write into file failed!!!");
    }
    else
    {
        printf("Written %lu byte to file\n", byteWritten);
    }

    close(fd);

    return 0;
}

