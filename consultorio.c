#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

int main(int argc, char *argv[]) {

// NO USAR VARIABLES GLOBALES, Y MIRAR RÚBRICA PARA HACERLO CORRECTAMENTE

	//El script pasa el argumento correctamente y lo imprime como un array de chars
	printf("Se está tensando, por lo que el número de pacientes es %s\n", argv[1]);

	//Ahora hacer que el parámetro pase a ser un int
	int parameter;
	//PROBAR CON ATOI, DEBERÍA DE FUNCIONAR PERFECTAMENTE
//	sscanf(argv[1], "%d", &parameter);
	//parameter(atoi(argv[1]));
	//printf("El número pasado como parámetro es: %d, ESTAMOS?!", &parameter);

return 0;
}

