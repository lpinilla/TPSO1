#include "queue.h"
#include "slave.h"

int main(void){
	//Una cola de los nombres de archivos y otra para los hashes
	Queue file_names, hashes;
	queueInit(&file_names, sizeof(char *));
	queueInit(&hashes, sizeof(char) * MD5LENGTH); //revisar si lo necesitamos
	read_file_names(file_names);


	//al finalizar hay que liberar la cola
	clearQueue(&file_names);
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
	//el hash lo devuelve tipo: HASH NOMBRE DEL ARCHIVO así que hay que parsear

	
}

void ask_for_more_files(Queue q){
	//avisarle al padre
	//read_file_names(q);
}

char * grab_hash(char * md5sum_output){
	//TODO: este vector esta en el stack, hay que fijarse si no va a molestar y
	//si hay que crear un char * con malloc
	char ret[MD5LENGTH * sizeof(char)]; 
	for(int i = 0; i < MD5LENGTH; i++){
		ret[i] = md5sum_output[i];
	} 
	return ret;
}