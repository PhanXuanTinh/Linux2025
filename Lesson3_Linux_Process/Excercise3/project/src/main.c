#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sigusr_handle(int sigusr)
{
    printf("Signal %d received\n", sigusr);
}

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process
        signal(SIGUSR1, sigusr_handle);

        printf("Child process: waiting for signal of parent process\n");

        pause();
        printf("Child process: received signal from parent process\n");
    }
    else
    {
        //Parent process
        sleep(5);

        printf("Parent process: sending signal to child process\n");
        kill(pid, SIGUSR1);

        wait(NULL); // Wait for child process to finish
        printf("Parent process: wait for child process finished\n");
    }

    return 0;
}
