#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 5000
#define CONSUME_SIZE 1000
#define NUM_THREADS 4

char buf[BUFFER_SIZE][CONSUME_SIZE];
int count[BUFFER_SIZE] = {};
const char targ[]="<http://www.w3.org/2001/XMLSchema#string>";
const char invalid[]={' ','\n', '<'};
int total_length;
FILE *fp; 
int sharedTotalCount=0;

typedef struct {
    size_t len;
    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
} buffer_t;

void* producer (void *arg) {
    buffer_t *buffer = (buffer_t*)arg;
    int len,i;
    while(1) {
        pthread_mutex_lock(&buffer->mutex);
        if(feof(fp)) {
            pthread_mutex_unlock(&buffer->mutex);
            for(i=0; i<NUM_THREADS; i++) {
                pthread_mutex_lock(&buffer->mutex);
                pthread_cond_signal(&buffer->can_consume);
                pthread_mutex_unlock(&buffer->mutex);
            }
            return NULL;
        }
        if(buffer->len == BUFFER_SIZE) {
            pthread_cond_wait(&buffer->can_produce, &buffer->mutex);
        }
        len = fread(buf[buffer->len], sizeof(char), CONSUME_SIZE, fp); 
        buf[buffer->len][len] = '\0';
        if(!feof(fp)) {
            while(!strchr(invalid, fgetc(fp))) {
                buf[buffer->len][--len] = '\0';
                fseek(fp, -2, SEEK_CUR);
            }
            fseek(fp,-1,SEEK_CUR);
        }
        ++buffer->len;
        pthread_cond_signal(&buffer->can_consume);
        pthread_mutex_unlock(&buffer->mutex);
    }
    pthread_exit(NULL);
}

void* consumer (void *arg) {
    buffer_t *buffer = (buffer_t*)arg;
    int len, i;
    while(1) {
        pthread_mutex_lock(&buffer->mutex);
        while(buffer->len == 0) {
            pthread_cond_wait(&buffer->can_consume, &buffer->mutex);
            if(feof(fp)) {
                pthread_cond_signal(&buffer->can_produce);
                pthread_mutex_unlock(&buffer->mutex);
                return NULL;
            }
        }
        --buffer->len;
        len = strlen(buf[buffer->len])-total_length+1;
        for(i=0;i<len;i++) {
            if(strncmp(&buf[buffer->len][i], targ, total_length)==0) 
                ++count[buffer->len];
        }
        pthread_cond_signal(&buffer->can_produce);
        pthread_mutex_unlock(&buffer->mutex);
    }
    pthread_exit(NULL);
}

int main (int argc, char *args[]) {
    if(argc != 2) {
        printf("Error! Filename not found!\n");
        exit(-1);
    }
    fp = fopen(args[1], "r");
    buffer_t buffer = {
        .len = 0,
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .can_produce = PTHREAD_COND_INITIALIZER,
        .can_consume = PTHREAD_COND_INITIALIZER
    };
    total_length = strlen(targ);
    pthread_t cons[NUM_THREADS], prod;
    time_t a, b;
    int i, sum = 0;
    printf("File name         : %s \n", args[1]);
    printf("Buffer size       : %dK\n", BUFFER_SIZE/1000);
    printf("Consume Size      : %dK\n", CONSUME_SIZE/1000);
    printf("Number of threads : %d\n", NUM_THREADS);
    printf("Find string \"<http://www.w3.org/2001/XMLSchema#string>\" match\n");
    printf("Searching...\n");
    a = time(NULL);
    pthread_create(&prod, NULL, producer, (void*)&buffer);
    for(i=0; i<NUM_THREADS; i++) {
        pthread_create(&cons[i], NULL, consumer, (void*)&buffer);
    }
    pthread_join(prod, NULL);
    for(i=0; i<NUM_THREADS; i++) {
        pthread_join(cons[i], NULL);
    }
    b = time(NULL);
    double search = difftime(b, a);
    printf("Searching time used %lf seconds.\n", search);
    for(i=0; i<BUFFER_SIZE; i++) {
        sum += count[i];
    }
    printf("Found string %d times\n", sum);
    pthread_exit(NULL);
}