/*****************************************************************************
*  This example code shows how to use most of PAPI's High level functions    * 
*  to start,count,read and stop on an event set. We use two preset events    *
*  here:                                                                     *
*     PAPI_TOT_INS: Total instructions executed in a period of time		     *
*     PAPI_TOT_CYC: Total cpu cycles in a period of time		             *
******************************************************************************/ 

#include <stdio.h>
#include <stdlib.h>
#include "papi.h"

#define NUM_EVENTS 2
#define THRESHOLD 1000000
#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n", retval,__FILE__,__LINE__);  exit(retval); }

/* stupid codes to be monitored */ 
void computation_mult()
{
   double tmp=1.0;
   int i=1;
   for( i = 1; i < THRESHOLD; i++ )
   {
      tmp = tmp*i;
   }
}

/* stupid codes to be monitored */ 
void computation_add()
{
   int tmp = 0;
   int i=0;

   for( i = 0; i < THRESHOLD; i++ )
   {
      tmp = tmp + i;
   }

}


int main()
{
   /*Declaring and initializing the event set with the presets*/
   int Events[2] = {PAPI_TOT_INS, PAPI_TOT_CYC};
   /*The length of the events array should be no longer than the 
     value returned by PAPI_num_counters.*/	
	
   /*declaring place holder for no of hardware counters */
   int num_hwcntrs = 0;
   int retval, retval2;
   char errstring[PAPI_MAX_STR_LEN];
   /*This is going to store our list of results*/
   long long values[NUM_EVENTS];

  float real_time, proc_time,mflops;
  long long flpops;
  float ireal_time, iproc_time, imflops;
  long long iflpops;
   /***************************************************************************
   *  This part initializes the library and compares the version number of the*
   * header file, to the version of the library, if these don't match then it *
   * is likely that PAPI won't work correctly.If there is an error, retval    *
   * keeps track of the version number.                                       *
   ***************************************************************************/

   if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT )
   {
      fprintf(stderr, "Error: %d %s\n",retval, errstring);
      exit(1);
   }

   /**************************************************************************
    * PAPI_num_counters returns the number of hardware counters the platform *
    * has or a negative number if there is an error                          *
    **************************************************************************/
   if ((num_hwcntrs = PAPI_num_counters()) < PAPI_OK)
   {
      printf("There are no counters available. \n");
      exit(1);
   }

   printf("There are %d counters in this system\n",num_hwcntrs);
		
   /**************************************************************************
    * PAPI_start_counters initializes the PAPI library (if necessary) and    *
    * starts counting the events named in the events array. This function    *
    * implicitly stops and initializes any counters running as a result of   *
    * a previous call to PAPI_start_counters.                                *
    **************************************************************************/
	
   if ( (retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK)
       ERROR_RETURN(retval);

   printf("\nCounter Started: \n");

   /* Your code goes here*/
   computation_add();
	

  

   /**********************************************************************
    * PAPI_read_counters reads the counter values into values array      *
    **********************************************************************/

   if ( (retval=PAPI_read_counters(values, NUM_EVENTS)) != PAPI_OK)
      ERROR_RETURN(retval);

   printf("Read successfully\n");



   printf("The total instructions executed for addition are %lld \n",values[0]);
   printf("The total cycles used are %lld \n", values[1] );
	
   printf("\nNow we try to use PAPI_accum to accumulate values\n"); 

   /* Do some computation here */
   computation_add();
     
 
   /************************************************************************
    * What PAPI_accum_counters does is it adds the running counter values  *
    * to what is in the values array. The hardware counters are reset and  *
    * left running after the call.                                         *
    ************************************************************************/

   if ( (retval=PAPI_accum_counters(values, NUM_EVENTS)) != PAPI_OK)
      ERROR_RETURN(retval);

   printf("We did an additional %d times addition!\n", THRESHOLD);
   printf("The total instructions executed for addition are %lld \n",
			values[0] );
   printf("The total cycles used are %lld \n", values[1] );
   printf("CPI = %lf \n", (double) values[1]/values[0] );

   /***********************************************************************
    * Stop counting events(this reads the counters as well as stops them  *
    ***********************************************************************/

   printf("\nNow we try to do some multiplications\n");
   computation_mult();

   /******************* PAPI_stop_counters **********************************/
   if ((retval=PAPI_stop_counters(values, NUM_EVENTS)) != PAPI_OK)
      ERROR_RETURN(retval);	

   printf("The total instruction executed for multiplication are %lld \n",
               values[0] );
   printf("The total cycles used are %lld \n", values[1] );

   printf("CPI = %lf \n", (double) values[1]/values[0] );

    /**** we compute flops ***/
  if((retval2=PAPI_flops(&ireal_time,&iproc_time,&iflpops,&imflops)) < PAPI_OK)
  {
    printf("Could not initialise PAPI_flops \n");
    printf("Your platform may not support floating point operation event.\n");
    printf("retval: %d\n", retval2);
    exit(1);
  }
  computation_mult();
  if((retval2=PAPI_flops(&real_time,&proc_time,&flpops,&mflops)) < PAPI_OK)
  {
    printf("retval: %d\n", retval2);
    exit(1);
  }

   printf("Real_time %f Proc_time: %f Total flpops: %lld MFLOPS: %f\n",
         real_time, proc_time,flpops,mflops);

   const PAPI_hw_info_t *hwinfo = NULL;

   if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT )
   {
      printf("Library initialization error! \n");
      exit(1);
   }

   /* Get hardware info*/
   if ((hwinfo = PAPI_get_hardware_info()) == NULL)
   {
      printf("PAPI_get_hardware_info error! \n");
      exit(1);
   }
   /* when there is an error, PAPI_get_hardware_info returns NULL */


   printf("%d CPU  at %f Mhz.\n",hwinfo->totalcpus,hwinfo->mhz);
   printf(" model string is %s \n", hwinfo->model_string);
   PAPI_shutdown(); 

   exit(0);	
}
