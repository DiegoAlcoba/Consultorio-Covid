#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/syscall.h>

int main(int argc, char *argv[]) {

	int parameter; 
	parameter = atoi(argv[1]); //Paso el argumento pasado a int

	pid_t pid;
	int *hijos;
	hijos = (int *) malloc ((3 * sizeof (int));

	for (int i = 0; i <= hijos; i++) {
		
		pid = fork();
	}


return 0;
}

