#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/syscall.h>

/*Función que calcula números aleatorios*/
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

	printf("** Responsable de inventario preparando las vacunas **\n...\n");
	sleep(calculaAleatorios(3, 6));
	
	exit(calculaAleatorios(0, 1));

}

/*Auxiliares reciben la orden de proceder con la vacunación*/
void handle_vacunacion(int sig) {

    /*Se vuelve a asignar el handler a la señal SIGUSR2*/
	if (signal(SIGUSR2, handle_vacunacion) == SIG_ERR) { 
		perror("Llamada a signal");			
		exit(-1);
	}


}

/*Auxiliares están vacunando en el momento*/
void handle_vacunando(int sig) {
	
	/*Se vuelve a asignar el handler a la señal SIGUSR1, esta vez para los pacientes por los auxiliares*/
	if (signal(SIGUSR1, handle_vacunando) == SIG_ERR) { 
		perror("Llamada a signal");
					
		exit(-1);
	}

	printf("** Paciente siendo vacunado **\n");
	sleep(calculaAleatorios(3, 6));
	printf("Paciente vacunado con éxito\n");

	exit(calculaAleatorios(0, 1));
}

int main(int argc, char *argv[]) {

	int parameter, estado, vacunas, i;
	int *pacientes; //Recordar liberar la memoria al final "free(pacientes)"

	/* Paso el argumento del programa (pacientes) como entero y creo un array dinámico del tamaño del número de pacientes*/ 

	parameter = atoi(argv[1]);
	pacientes = (int *) malloc (parameter * sizeof (int));

	/*Array que contendrá a los hijos*/
	int *hijos;
	hijos = (int *)malloc(3 * sizeof (int));

	pid_t p, pp;

	/*Creación del responsable de inventario y los dos auxiliares*/
	for (i = 0; i < 3; i++) {

	    p = fork();

		if (p == -1) {

			perror("Error en la llamada a fork()\n");

			exit(-1);
		} 
		else if (p == 0) {

			switch (i) {
				case 0: /*Responsable del inventario*/
					
					/*Se le asignal el handler a la señal SIGUSR1*/
					if (signal(SIGUSR1, handle_preparaVacunas) == SIG_ERR) { 
						perror("Llamada a signal");
					
						exit(-1);
					}

					pause();

					break;
				case 1: /*Auxiliar 1*/
				case 2: /*Auxiliar 2*/

					/*Se les asigna el handler a la señal SIGUSR2*/
					if (signal(SIGUSR2, handle_vacunacion) == SIG_ERR) { 
						perror("Llamada a signal");
					
						exit(-1);
					}
					
					/*Cada auxiliar crea n/2 procesos si pacientes pares*/
					if (parameter % 2 == 0) {
							
						for (i = 0; i < (parameter / 2); i++) {
							
							pp = fork();

							if (pp == -1) {
								perror("Error en la llamada a fork()\n");
							}
							else if (pp == 0){ /*Pacientes*/

								/*Se les asigna el handler a la señal SIGUSR1*/
								if (signal(SIGUSR1, handle_vacunando) == SIG_ERR) { 
									perror("Llamada a signal");
					
									exit(-1);
								}

								pause(); /*Esperan a que el auxiliar les envíe la señal*/
							}
							else { /*Auxiliar*/

								pacientes[i] = pp;	
							}
						}
					}

					/*Un auxiliar crea n/2 procesos, y el otro (n/2) + 1 si pacientes son impares*/
					if (parameter % 2 != 0) {
							
						for (i = 0; i < (parameter / 2); i++) {
							
							pp = fork();

							if (pp == -1) {
								perror("Error en la llamada a fork()\n");
							}
							else if (pp == 0){ /*Pacientes*/

								/*Se les asigna el handler a la señal SIGUSR1*/
								if (signal(SIGUSR1, handle_vacunando) == SIG_ERR) { 
									perror("Llamada a signal");
					
									exit(-1);
								}

								pause(); /*Esperan a que el auxiliar les envíe la señal*/
							}
							else { /*Auxiliar*/

								pacientes[i] = pp;	
							}
						}
					}

					/*
					*
					*
					* CÓDIGO, BÁSICAMENTE EL DE ARRIBA PERO A UN AUX N/2 + 1
					*
					*/
					pause();/*Los auxiliares esperan a recibir la señal del coordinador*/

					/*Vacunación*/
					//printf("** Auxiliar preparando la vacuna **\n");  
					//sleep(calculaAleatorios(2, 5));
						
					int reacciones = 0;	
					/*Envía la señal a cada uno de los pacientes asignados*/
					for (i = 0; i < (parameter / 2); i++) {

						printf("\n** Auxiliar preparando la vacuna **\n...\n");  
						sleep(calculaAleatorios(2, 5));
						
						kill(pacientes[i], SIGUSR1);			
					
						wait(&estado);

						int react = WEXITSTATUS(estado);

						for (int j = 0; j < (parameter / 2); j++) {
							if (react == 1) {
								reacciones = reacciones + 1;
							}
						}
					}

					exit(reacciones);
					break;
			}
		} 
		else { /*Coordinador*/

            hijos[i] = p;
		}
	}

	kill(hijos[0], SIGUSR1);
	wait(&estado);

	int ret = WEXITSTATUS(estado);
	int reaccionesTotales = 0;

	if (ret == 0) { /*No hay vacunas suficientes*/

		for (int i = 0; i < 3; i++) {
			kill(hijos[i], SIGTERM);
		}

		printf("\n** No hay vacunas suficientes, abortando **\n"); 
		exit(0);
	}
	else if (ret == 1) { /*Hay vacunas suficientes*/
		
		printf("\n** Autorización concedida. Procediendo a la vacunación **\n");

		for (int i = 1; i < 3; i++) {
			
			kill(hijos[i], SIGUSR2);
			wait(&estado);
	
			for (int i = 1; i < 3; i++) {
				reaccionesTotales = WEXITSTATUS(estado) + reaccionesTotales;
			}
		}
		
		printf("\n%d pacientes han tenido reacción a la vacuna.\n", reaccionesTotales);
	}
	else {
		perror("** Ha ocurrido algo inesperado**\n");
	}

	free(hijos);
	free(pacientes);
	
	return 0;
}
