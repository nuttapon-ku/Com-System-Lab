#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define SHM_SIZE 8

/* let us make a recursive function to print the content of a given folder */

long child_process(char *path);
long count_dir_content(char *path) {
    long counter = 0;
    DIR * d = opendir(path); // open the path
    if(d==NULL) return 0; // if was not able return
    struct dirent * dir; // for the directory entries
    while ((dir = readdir(d)) != NULL) { // if we were able to read somehting from the directory
        if(dir-> d_type != DT_DIR) { // if the type is not directory just print it with blue
            counter = counter + 1;
        }
        else
            if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 ) { // if it is a directory
                char d_path[255]; // here I am using sprintf which is safer than strcat
                sprintf(d_path, "%s/%s", path, dir->d_name);
                int fd[2];
                pid_t pid;
                pipe(fd);
                pid = fork();
                int child_count = 0;

                if(pid == 0) {
                    close(fd[0]); // close up input side of the pipe
                    child_count = child_count + count_dir_content(d_path); // recall with the new path
                    write(fd[1], &child_count, sizeof(child_count)); // send child_count to parent
                    exit(0);
                } else {
                    close(fd[1]); // close up output side of the pipe
                    int returnStatus;
                    waitpid(pid, &returnStatus, 0); // wait for little child to terminate
                    read(fd[0], &child_count, sizeof(child_count)); // get counter value from child
                    counter = counter + child_count;
                }
            }
    }
    closedir(d); // finally close the directory
    return counter;
}

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("wrong input: ./prob3a [DIR]\n");
        exit(0);
    }
    long N = count_dir_content(argv[1]);
    printf("The number of files: %ld\n", N);
    return(0);
}