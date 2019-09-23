#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    int shm;
    if ((shm = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(-1);
    }
    
    int * shared;
    if ((shared = shmat(shm, NULL, 0)) == (void *) -1) {
        perror("shmat");
        exit(-1);
    }

    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(-1);
    }


    if (pid == 0) {
        puts("Child");

        *shared = 123;
        printf("Child: *shared = %d\n", *shared);

        sleep(2);

        printf("Child: *shared == %d\n", *shared);

        exit(0);
    }

    puts("Parent");

    sleep(1);

    printf("Parent: *shared == %d\n", *shared);
    *shared = 456;
    printf("Parent: *shared = %d\n", *shared);

    sleep(2);

    printf("Parent: *shared == %d\n", *shared);
}
