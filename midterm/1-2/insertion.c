#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define len 100100
#define FILE_NAME "data1d-100000"

int n=0, *array;

int main() {
    array = malloc(sizeof(int)*len);
    FILE *file;
    file = fopen(FILE_NAME, "rb");
    if (!file) {
        perror ("Cannot opening file.");
        return -1;
    }

    ///////////////////////////////////////////////

    memset(array, 0, len*sizeof(int));
    while (!feof(file) && n<len) {
        fread(&array[n], sizeof(int), 1, file);
        n++;
    }
    n--;
    fclose (file);

    ///////////////////////////////////////////////
    int c, d, t;
    for(c = 0; c < n; c++) {
        d = c;
        while(d > 0 && array[d] < array[d-1]) {
            t = array[d];
            array[d] = array[d-1];
            array[d-1] = t;
            d--;
        }
    }
    printf("Sorted list in ascending order:\n");
    for(c=0; c<n; c++) {
        printf("%d\n", array[c]);
    }
    return 0;
}
