#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "papi.h"

#define NUM_EVENTS 2
#define THRESHOLD 1000000
#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n", retval,__FILE__,__LINE__);  exit(retval); }
#define DATA_FILE "matData/mat1000.dat"


int n;

void computation_mult() {
    FILE *file;
    file = fopen (DATA_FILE, "r");
    if (!file) {
        perror ("Error opening file.");
    }

    fscanf (file, "%d", &n);
    int count = 0;
    float A[n][n], AA[n][n];
    memset(A, 0, sizeof(A));
    while (!feof(file) && count < n) {
        for(int j = 0; j < n; j++) {
            fscanf (file, "%f", &A[count][j]);
        }
        count++;
    }
    fclose (file);

    memset(AA, 0, sizeof(AA));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            for(int k = 0; k < n; k++) {
                AA[i][j] += A[i][k] * A[k][j];
            }
        }
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

    printf("\nMatrix multiplication for 1000x1000:\n");
    computation_mult();

    if ((retval=PAPI_stop_counters(values, NUM_EVENTS)) != PAPI_OK)
        ERROR_RETURN(retval);	

    printf("The total instruction executed for multiplication are %lld \n",
            values[0] );
    printf("The total cycles used are %lld \n", values[1] );

    printf("CPI = %lf \n", (double) values[1]/values[0] );

    if((retval2=PAPI_flops(&ireal_time,&iproc_time,&iflpops,&imflops)) < PAPI_OK) {
        printf("Could not initialise PAPI_flops \n");
        printf("Your platform may not support floating point operation event.\n");
        printf("retval: %d\n", retval2);
        exit(1);
    }
    computation_mult();
    if((retval2=PAPI_flops(&real_time,&proc_time,&flpops,&mflops)) < PAPI_OK) {
        printf("retval: %d\n", retval2);
        exit(1);
    }

    printf("Real_time %f Proc_time: %f Total flpops: %lld MFLOPS: %f\n",
            real_time, proc_time,flpops,mflops);
    long long mips = values[0] / 1000000.0 / real_time;
    printf("MIPS: %lld\n", mips);

    const PAPI_hw_info_t *hwinfo = NULL;

    if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT ) {
        printf("Library initialization error! \n");
        exit(1);
    }
    if ((hwinfo = PAPI_get_hardware_info()) == NULL) {
        printf("PAPI_get_hardware_info error! \n");
        exit(1);
    }
    printf("%d CPU  at %f Mhz.\n",hwinfo->totalcpus,hwinfo->mhz);
    printf(" model string is %s \n", hwinfo->model_string);
    PAPI_shutdown(); 
    exit(0);	
}