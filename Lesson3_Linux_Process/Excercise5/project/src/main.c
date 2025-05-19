#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// #define CREATE_ZOMBIE 1
#define CREATE_ORPHANS 2

int main() {
    pid_t pid = fork();

#ifdef CREATE_ZOMBIE
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process: terminate immediately
        printf("Zombie Child (PID %d) terminates immediately\n", getpid());
        exit(0);
    } else {
        // Parent process: sleep for some time without calling wait()
        printf("Zombie Parent (PID %d) sleeping to leave child as zombie\n", getpid());
        sleep(10);
        // Now collect child to clean zombie, so zombie stays only briefly
        wait(NULL);
        printf("Zombie Parent collected child, zombie cleared\n");
    }

#elif defined(CREATE_ORPHANS)
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process: sleep longer to become orphan
        printf("Orphan Child (PID %d) running and sleeping\n", getpid());
        sleep(10);
        printf("Orphan Child (PID %d) woke up, now exiting\n", getpid());
        exit(0);
    } else {
        // Parent process: terminate quickly
        printf("Orphan Parent (PID %d) terminates immediately\n", getpid());
        exit(0);
    }
#endif

    return 0;
}