#include <unistd.h>
#include <stdio.h>
void main()
{
	int fda[2];  // file descriptors
    int fda1[2]; // file descriptors
    if(pipe(fda) < 0) printf("create pipe failed\n");
    switch(fork())
    {
    	case -1: printf("fork failed\n");
    	case 0: // run ls in child process
			if(pipe(fda1) < 0) printf("create pipe failed\n");
			switch (fork())
			{
				case -1: printf("child fork failed\n");
				case 0:
	    			close(1);
	    			dup(fda1[1]);
	    			close(fda1[0]);
	    			close(fda1[1]);
	    			printf("in child\n");
	    			execlp("cat", "cat", "4300.txt", (char *)0);
	    			printf("failed to exec ls\n");
	    			break;
				default:
	    			close(0);				      
	    			dup(fda1[0]);			      
	    			close(1);				      
	    			dup(fda[1]);			      
	    			close(fda[0]);			      
	    			close(fda[1]);	
	    			close(fda1[0]);	
	    			close(fda1[1]);	
					execlp("grep", "grep", "ant", (char *) 0);
	     			printf("failed to execute wc\n");
	     			break;
			}

		default:
     		close(0);
     		dup(fda[0]);
     		close(fda[0]);
     		close(fda[1] );
			execlp("wc", "wc", (char *) 0);
     		printf("failed to execute wc\n");
     		break;
     }		
}