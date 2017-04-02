#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 5000
#define NUM_THREADS 4

const char targ[] = "<http://www.w3.org/2001/XMLSchema#string>";
const char invalid[] = {' ','\n', '<'};

int sharedByteRead = 0;
int sharedTotalCount = 0;
int total_length;


FILE *fp; // keep file pointer for the text file
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* GetDataBytes() {
    char data[BUFFER_SIZE + 1];
    int len, i, count=0;
    while(1) {
        pthread_mutex_lock(&mutex);
        if(feof(fp)) 
            break;
        len = fread(data, sizeof(char), BUFFER_SIZE, fp);
        data[len] = '\0';
        if(!feof(fp)) {
            while(!strchr(invalid, fgetc(fp))) {
                data[--len] = '\0';
                fseek(fp, -2, SEEK_CUR);
            }
            fseek(fp, -1, SEEK_CUR);
        }
        pthread_mutex_unlock(&mutex);
        len = len-total_length+1;
        for(i=0; i<len; i++) {
            if(strncmp(&data[i], targ, total_length)==0) 
                ++count;
        }
    }
    pthread_mutex_unlock(&mutex);
    pthread_mutex_lock(&mutex2);
    sharedTotalCount += count;
    pthread_mutex_unlock(&mutex2);
    pthread_exit(NULL);
}

int main(int argc, char** args) {
    // read filename
    if(argc != 2) {
        printf("Error! Filename not found!\n");
        exit(-1);
    }
    //open file
    fp = fopen(args[1], "r");
    pthread_t threads[NUM_THREADS];
    int rc;
    long t=0;
    time_t a, b;
    total_length = strlen(targ);
    printf("File name         : %s \n", args[1]);
    printf("Buffer size       : %dK\n", BUFFER_SIZE/1000);
    printf("Number of threads : %d\n", NUM_THREADS);
    printf("Find string \"<http://www.w3.org/2001/XMLSchema#string>\" match\n");
    a = time(NULL);
    printf("Searching...\n");
    for(t=0; t<NUM_THREADS; t++) {
       rc = pthread_create(&threads[t], NULL, GetDataBytes, NULL);
       if (rc) {
          printf("ERROR! Code from pthread_create %d\n", rc);
          exit(-1);
       }
    }
    for(t=0; t<NUM_THREADS; t++) {
        if(pthread_join(threads[t], NULL)) {
            printf("ERROR! Cannot join threads\n");
            exit(-1);
        }
    }
    b = time(NULL);
    double search = difftime(b, a);
    printf("Searching time used %lf seconds.\n", search);
    printf("Found string %d times\n", sharedTotalCount);
    pthread_exit(NULL);
}