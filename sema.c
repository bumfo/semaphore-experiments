#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

int main() {
    int shm;
    if ((shm = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(-1);
    }

    sem_t * sem;
    if ((sem = shmat(shm, NULL, 0)) == (void *) -1) {
        perror("shmat");
        exit(-1);
    }

    if (sem_init(sem, 1, 0) == -1) {
        perror("sem_init");
        exit(-1);
    }

    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(-1);
    }

    if (pid == 0) {
        puts("Child");

        while (1) {
            sem_wait(sem);

            puts("Child exec");
        }

        exit(0);
    }

    puts("Parent");

    while (1) {
        sleep(1);
        puts("Parent exec");

        sem_post(sem);
    }
}
