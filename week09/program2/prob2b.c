#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define WRFLAGS (O_WRONLY | O_CREAT | O_TRUNC)
#define MODE600 (S_IRUSR | S_IWUSR)

int main() {
	int fda[2];
	int in, out;
	if(pipe(fda) < 0)
		printf("create pipe failed\n");
    if (fork() == 0) {
		close(0);
		if (open("4300.txt", O_RDONLY) == -1)
			printf ("open input file failed\n");		
		close(1);
		dup(fda[1]);
		close(fda[0]);
		close(fda[1]);
        execlp("wc", "wc", (char *) 0);
    }
	else {
		close(0);
		dup(fda[0]);
		close(1);
		if (open("tt", WRFLAGS, MODE600) == -1)
			printf("open output failed\n"); 
		close(fda[0]);
		close(fda[1]);
    	execlp("cat", "cat", (char *) 0);
	}
}