#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <semaphore.h>

class ipcsema {
    sem_t * sem;

public:
    ipcsema() {
        int shm_id;
        if ((shm_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666)) < 0) {
            perror("ipcsema: shmget");
            exit(-1);
        }

        if ((sem = (sem_t *) shmat(shm_id, NULL, 0)) == (void *) -1) {
            perror("ipcsema: shmat");
            exit(-1);
        }

        if (sem_init(sem, 1, 0) == -1) {
            perror("ipcsema: sem_init");
            exit(-1);
        }
        
        if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
            perror("ipcsema: shmctl IPC_RMID");
            exit(-1);
        }
    }

    void post() {
        sem_post(sem);
    }

    void wait() {
        sem_wait(sem);
    }

    ~ipcsema() {
        sem_destroy(sem);

        if (shmdt(sem) == -1) {
            perror("~ipcsema: shmdt");
            exit(-1);
        }
    }
};

class child {
    pid_t pid;

public:
    template <typename F>
    child(F f) {
        if ((pid = fork()) < 0) {
            perror("child: fork");
            exit(-1);
        }

        if (pid == 0) {
            f();

            exit(0);
        }
    }

    int wait() {
        int status;
        waitpid(pid, &status, 0);

        return status;
    }
};
