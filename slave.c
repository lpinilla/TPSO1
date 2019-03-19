#include "queue.h"


//función para que lea los procesos que el padre le envió
void read_file_names(Queue q);

//función para calcular el hash md5 en base a un archivo
int process_hash(char * file_name); //creo que devuelve int

//función para que le indique al padre que puede recibir más archivos
void ask_for_more_files(void);

int main(void){
	Queue q; //la cola de archivos del proceso esclavo

}

void read_file_names(Queue q){
	//read from stdin (with pipe from parent)
}

int process_hash(char * file_name){
	//call md5sum
}

void ask_for_more_files(Queue q){
	//avisarle al padre
	//read_file_names(q);
}