#include "queue.h"

#define MD5LENGTH 32

//función para que lea los procesos que el padre le envió
void read_file_names(Queue q);

//función para calcular el hash md5 en base a un archivo
int process_hash(char * file_name); //creo que devuelve int

//función para que le indique al padre que puede recibir más archivos
void ask_for_more_files(void);

int main(void){
	//Una cola de los nombres de archivos y otra para los hashes
	Queue file_names, hashes;
	queueInit(&file_names, sizeof(char *));
	queueInit(&hashes, sizeof(char) * MD5LENGTH);
}

//leer los nombres de los archivos de stdin
void read_file_names(Queue q){
	char * line = NULL;
	ssize_t linecap = 0, linelen;
	while( (linelen = getline(&line, &linecap, stdin)) > 0){ 
		q.enqueue(&q, &line);
	}
}

int process_hash(char * file_name){
	//call md5sum, hay que redireccionar el stdout para que vuelva el hash
}

void ask_for_more_files(Queue q){
	//avisarle al padre
	//read_file_names(q);
}