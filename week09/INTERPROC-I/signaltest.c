#include <stdio.h>	
#include <signal.h>	
#include <unistd.h>	
int ctrl_C_count = 0;
void (* old_handler)(int);	
void ctrl_c(int);	
int ctrl_c_count;
main () {	int c;		
	old_handler = signal (SIGINT, ctrl_c );		
	while ((c = getchar()) != '\n');		
	printf("ctrl_c count = %d\n", ctrl_c_count);		
	(void) signal (SIGINT, old_handler);

		for (;;);
}
void ctrl_c(int signum) {		
		(void) signal (SIGINT, ctrl_c);		// signals are automatically reset	
			++ctrl_c_count;	
}
