#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid > 0)
    {
        //Parent process
        wait(NULL);
        printf("<Parent process> The child process end\n");
    }
    else if (pid  == 0)
    {
        //Child process
        char *action = getenv("ACTION");
        if (action == NULL)
        {
             printf("Environment variable ACTION has not been set\n");
             exit(1);
        }

        if (strcmp(action, "1") == 0)
        {
            execlp("ls", "ls", NULL);
        }
        else if (strcmp(action, "2") == 0)
        {
            execlp("date", "date", NULL);
        }
        else
        {
            printf("ACTION invalid, it must be 1 or 2\n");
            exit(1);
        }

        perror("execl failed\n");
        exit(1);
    }
    else
    {
        perror("Fork failed\n");
    }

    return 0;
}