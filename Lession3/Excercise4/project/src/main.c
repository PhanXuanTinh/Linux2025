#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();
    int status;


    if (pid < 0)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        //Child process
        printf("Child process: PID = %d\n", getpid());

        exit (3);
    }
    else
    {
        wait(&status); // Wait for child process to finish
        
        if (WIFEXITED(status))
        {
            //Child process terminated nomally
            int exit_status = WEXITSTATUS(status);
            printf("Parent process: Child process terminated nomally with status: %d\n", exit_status);
        }
        else
        {
            //Child process terminated abnormally
            printf("Parent process: Child process terminated abnormally\n");
        }
    }

    return 0;
}
