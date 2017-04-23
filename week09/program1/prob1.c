#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define NITER 1000000
#define SHM_SIZE 8

sem_t mutex;

void * Count(void * a)
{
    int shmid;
    long *shm, *count;
    key_t key = 3541;

    // locate the segment
    if((shmid = shmget(key, SHM_SIZE, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // attach the segment
    if((shm = shmat(shmid, NULL, 0)) == (long *) -1) {
        perror("shmat");
        exit(1);
    }
    count = shm;

    int i, tmp;
    for(i = 0; i < NITER; i++)
    {
        sem_wait(&mutex);

        tmp = *count;      /* copy the shm count locally */
        tmp = tmp+1;    /* increment the local copy */
        *count = tmp;      /* store the local value into the shm count */

        sem_post(&mutex);
    }

    // detach from shared memory
    shmdt(shm);
}

int main(int argc, char * argv[])
{
    pthread_t tid1, tid2;

    sem_init(&mutex, 0, 1);

    /* SHARE MEMORY CODE  */

    int shmid;
    long *shm, *count;
    key_t key = 3541;

    // create the segment
    if((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // attach the segment
    if ((shm = shmat(shmid, NULL, 0)) == (long *) -1) {
        perror("shmat");
        exit(1);
    }

    // put count data into memory
    count = shm;
    *count = 0;

    /* CREATE THE THREAD  */

    if(pthread_create(&tid1, NULL, Count, NULL))
    {
        printf("\n ERROR creating thread 1");
        exit(1);
    }

    if(pthread_create(&tid2, NULL, Count, NULL))
    {
        printf("\n ERROR creating thread 2");
        exit(1);
    }

    if(pthread_join(tid1, NULL))	/* wait for the thread 1 to finish */
    {
        printf("\n ERROR joining thread");
        exit(1);
    }

    if(pthread_join(tid2, NULL))        /* wait for the thread 2 to finish */
    {
        printf("\n ERROR joining thread");
        exit(1);
    }

    if (*count < 2 * NITER)
        printf("\n BOOM! cnt is [%ld], should be %d\n", *count, 2*NITER);
    else
        printf("\n OK! cnt is [%ld]\n", *count);

    pthread_exit(NULL);

    // detach from shared memory
    shmdt(shm);
}

