#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#define SHM_SIZE 8

sem_t mutex;

/* let us make a recursive function to print the content of a given folder */

void child_process(char * path);

void count_dir_content(char * path) {
    DIR * d = opendir(path); // open the path
    if(d==NULL) return; // if was not able return
    struct dirent * dir; // for the directory entries
    int shmid;
    long *shm, *count;
    key_t key = 3166;
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
    while ((dir = readdir(d)) != NULL) { // if we were able to read somehting from the directory
        if(dir-> d_type != DT_DIR) { // if the type is not directory
            sem_wait(&mutex);
            int tmp = *count;
            tmp = tmp + 1;
            *count = tmp;
            sem_post(&mutex);
        }
        else {
            if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0) { // if it is a directory
                char d_path[255]; // here I am using sprintf which is safer than strcat
                sprintf(d_path, "%s/%s", path, dir->d_name);
                pid_t pid;
                pid = fork();
                if(pid == 0) { // ChildProcess
                    child_process(d_path); // recall with the new path in child process
                } else { // Parent Process
                    int returnStatus;
                    waitpid(pid, &returnStatus, 0); // wait for little child to terminate
                }
            }
        }
    }
    shmdt(shm);
    closedir(d); // finally close the directory
}

void child_process(char * path) {
    count_dir_content(path);
    exit(0);
}

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("usage: ./prob3a [DIR]\n");
        exit(0);
    }
    sem_init(&mutex, 0, 1);
    /* SHARED MEMORY CODE  */
    int shmid;
    long *shm, *count;
    key_t key = 3166;
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
    /* END SHARED MEMORY CODE  */
    count_dir_content(argv[1]);
    printf("The number of files is: %ld\n", *count);
    shmdt(shm);
    return(0);
}