#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/syscall.h>

int main(int argc, char *argv[]) {

	//El script pasa el argumento correctamente y lo imprime como un array de chars
	printf("Se está tensando, por lo que el número de pacientes es %s\n", argv[1]);

	//Ahora hacer que el parámetro pase a ser un int
	int parameter;

return 0;
}

