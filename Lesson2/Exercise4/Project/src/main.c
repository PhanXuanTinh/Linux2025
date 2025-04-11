#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>


int main()
{
    const char *filename = "./test.txt";
    struct stat file_stat;

    if (stat(filename, &file_stat) == -1)
    {
        perror("stat failed");
        return 1;
    }

    printf("File name: %s\n", filename);

    printf("File type: ");
    if (S_ISREG(file_stat.st_mode)) {
        printf("Regular file\n");
    } else if (S_ISDIR(file_stat.st_mode)) {
        printf("Directory\n");
    } else if (S_ISLNK(file_stat.st_mode)) {
        printf("Symbolic link\n");
    } else {
        printf("Other\n");
    }

    printf("File size: %ld bytes\n", file_stat.st_size);
    printf("Last modify time: %s", ctime(&file_stat.st_mtime));

    return 0;
}
