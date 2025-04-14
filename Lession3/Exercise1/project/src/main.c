#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid = fork();

    if (pid > 0)
    {
        //Parent process
        printf("<Parent process> The child process'PId: %d\n", pid);
    }
    else if(pid == 0)
    {
        //Child process
        printf("<Child process> My PId: %d\n", getpid());
    }
    else
    {
        perror("Fork failed");
    }

    return 0;
}