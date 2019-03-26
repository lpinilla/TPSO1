#include "slave.h"
#include "utilities/utils.h"

int main(void){
	//Una cola de los nombres de archivos y otra para los hashes
	Queue file_names;
	queueInit(&file_names, sizeof(char *));
	//read_file_names(file_names);


	//al finalizar hay que liberar la cola
	clearQueue(&file_names);
}
