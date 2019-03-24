#include "slave.h"

int main(void){
	//Una cola de los nombres de archivos y otra para los hashes
	Queue file_names;
	//Queue hashes;
	queueInit(&file_names, sizeof(char *));
	//queueInit(&hashes, sizeof(char) * MD5LENGTH); //revisar si lo necesitamos
	read_file_names(file_names);


	//al finalizar hay que liberar la cola
	clearQueue(&file_names);
	//clearQueue(&hashes);
}

//leer los nombres de los archivos de stdin
void read_file_names(Queue q){
	char * line = NULL;
	ssize_t linecap = 0;
	size_t linelen;
	while( (linelen = getline(&line, (size_t *) &linecap, stdin)) > 0){ 
		enqueue(&q, &line);
	}
}

void process_hash(char * file_name){
	char * command_name = "md5sum";
	char * full_command; //combinar para que sea "md5sum file_name"
	char * command_output; //calcular bien el espacio
	call_command(full_command, command_output);
	return;
}

void ask_for_more_files(Queue q){
	//avisarle al padre
	//read_file_names(q);
}
