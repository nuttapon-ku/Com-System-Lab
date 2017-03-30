#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "papi.h"

#define len 100100
#define FILE_NAME "data1d-100000"
#define NUM_EVENTS 2
#define THRESHOLD 1000000
#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n", retval,__FILE__,__LINE__);  exit(retval); }

int *array, n = 0;

void insertion_sort()
{
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
}

int main() {
    int Events[2] = {PAPI_TOT_INS, PAPI_TOT_CYC};
    int num_hwcntrs = 0;
    int retval, retval2;
    char errstring[PAPI_MAX_STR_LEN];
    long long values[NUM_EVENTS];

    float real_time, proc_time,mflops;
    long long flpops;
    float ireal_time, iproc_time, imflops;
    long long iflpops;
    if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT ) {
        fprintf(stderr, "Error: %d %s\n",retval, errstring);
        exit(1);
    }
    if ((num_hwcntrs = PAPI_num_counters()) < PAPI_OK) {
        printf("There are no counters available. \n");
        exit(1);
    }
    printf("There are %d counters in this system\n",num_hwcntrs);
    if ( (retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK)
        ERROR_RETURN(retval);
    printf("\nCounter Started: \n");
    if ( (retval=PAPI_read_counters(values, NUM_EVENTS)) != PAPI_OK)
        ERROR_RETURN(retval);
    printf("Read successfully\n");
    printf("\nInsertion sort for 400,000 bytes data:\n");
    insertion_sort();
    /******************* PAPI_stop_counters **********************************/
    if ((retval=PAPI_stop_counters(values, NUM_EVENTS)) != PAPI_OK)
        ERROR_RETURN(retval);	
    printf("The total instruction executed for insertion sort are %lld \n", values[0] );
    printf("The total cycles used are %lld \n", values[1] );
    printf("CPI = %lf \n", (double) values[1]/values[0] );
    /**** we compute flops ***/
    if((retval2=PAPI_flops(&ireal_time,&iproc_time,&iflpops,&imflops)) < PAPI_OK) {
        printf("Could not initialise PAPI_flops \n");
        printf("Your platform may not support floating point operation event.\n");
        printf("retval: %d\n", retval2);
        exit(1);
    }
    const PAPI_hw_info_t *hwinfo = NULL;
    if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT ) {
        printf("Library initialization error! \n");
        exit(1);
    }
    /* Get hardware info*/
    if ((hwinfo = PAPI_get_hardware_info()) == NULL) {
        printf("PAPI_get_hardware_info error! \n");
        exit(1);
    }
    /* when there is an error, PAPI_get_hardware_info returns NULL */
    printf("%d CPU  at %f Mhz.\n",hwinfo->totalcpus,hwinfo->mhz);
    printf(" model string is %s \n", hwinfo->model_string);
    PAPI_shutdown(); 
    exit(0);	
}