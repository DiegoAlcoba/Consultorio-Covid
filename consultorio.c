#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/syscall.h>

void preparaVacunas(pid_t c, pid_t r,int v) {
	
	printf("El número de vacunas disponibles son %d\n", v);	

}

int main(int argc, char *argv[]) {
	
	srand(getpid());

	int parameter, vacunas, estado;
	int *pacientes; //Recordar liberar la memoria al final "free(pacientes)"
	pid_t pidCoord, pidResp, pidAux1, pidAux2;

	// Paso el argumento del programa (pacientes) como entero y creo un array dinámico del tamaño del número de pacientes 
	parameter = atoi(argv[1]);
	pacientes = (int *) malloc (parameter * sizeof (int));

	vacunas = rand(); //Número vacunas disponibles aleatoria

	//Creación del responsable de inventario y los dos auxiliares
	//int *hijos;
	//hijos = (int *) malloc (3 * sizeof (int));
	pidCoord = wait(&estado);

	for (int i = 0; i <= 2; i++) {
		pidCoord = fork();
		
		if (pidCoord == -1) {
			perror("Error en la llamada a fork()");
		} else if (pidCoord == 0) {
			switch (i) {
				case 0:
				//printf("Soy el responsable de inventario %d\n", getpid());
					pidResp = getpid();
					sleep(3);
					break;
				case 1:
					//printf("Soy el aux1 %d\n", getpid());
					pidAux1 = getpid();
					sleep(3);
					break;
				case 2:
					//printf("Soy el aux2 %d\n", getpid());
					pidAux2 = getpid();
					sleep(3);
					break;
			}
		} else {
			wait(&estado);
			//printf("Soy el coordinador %d\n", getpid());
		}
	}

	//Llamo a la función para que el coordinador prepare las vacunas
	preparaVacunas(pidCoord, pidResp, vacunas);

return 0;
}
