#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/syscall.h>

/*Función que calcula aleatoriamente si hay vacunas suficientes o no*/
int calculaAleatorios(int min, int max) {
	srand(getpid());

	return (rand() % (max - min + 1) + min);
}

/*Preparación de las vacunas*/
void handle_preparaVacunas(int sig) {

	/*Se vuelve a asignar el handler a la señal SIGUSR1*/
	if (signal(SIGUSR1, handle_preparaVacunas) == SIG_ERR) {
		perror("Llamada a signal");
		exit(-1);
	}

	return calculaAleatorios(0, 1);
}

/*Auxiliares reciben la orden de proceder con la vacunación*/
void handle_vacunacion(int sig) {

	printf("** Procediendo a la vacunación **");
}

int main(int argc, char *argv[]) {

	int parameter, estado, vacunas;
	int *pacientes; //Recordar liberar la memoria al final "free(pacientes)"

	/* Paso el argumento del programa (pacientes) como entero y creo un array dinámico del tamaño del número de pacientes*/ 
	parameter = atoi(argv[1]);
	pacientes = (int *) malloc (parameter * sizeof (int));

	/*Array que contendrá a los hijos*/
	int *hijos;
	hijos = (int *)malloc(3 * sizeof (int));

	/*Creación del responsable de inventario y los dos auxiliares*/
	pid_t p;

	for (int i = 0; i < 3; i++) {
		
		p = fork();
		hijos[i] = p;
		
		if (p == -1) {
			perror("Error en la llamada a fork()\n");
		} 
		else if (p == 0) {
			switch (i) {
				case 0: /*Responsable del inventario*/
					
					signal(SIGUSR1, &handle_preparaVacunas); /*Recibe la señal*/
					
					/*Se le asignal el handler a la señal SIGUSR1*/
					if (signal(SIGUSR1, handle_preparaVacunas) == SIG_ERR) { 
						perror("Llamada a signal");
					
						exit(-1);
					}

					printf("** Responsable de inventario preparando las vacunas **\n");
					sleep(calculaAleatorios(3, 6));
					
					exit(handle_preparaVacunas(vacunas));
					break;
				case 1: /*Auxiliar 1*/
				case 2: /*Auxiliar 2*/

					signal(SIGUSR2, &handle_vacunacion);	
					
					break;
			}
		} 
		else { /*Coordinador*/

			kill(hijos[0], SIGUSR1);
			wait(&estado);

			if (WEXITSTATUS == 0) { /*No hay vacunas suficientes*/
				for (int i = 0; i < 3; i++) {
					kill(hijos[i], SIGTERM);
				}

				exit(-1);
			}
			else if (WEXITSTATUS == 1) { /*Hay vacunas suficientes*/
				
				for (int i = 1; i < 3; i++) {
					kill(hijos[i], SIGUSR2);
				}
				

			}
			else {
				perror("** Error en el número de vacunas **\n");
			}

		}
	}

return 0;
}
