#include "slave.h"

int main(void){
	//Una cola de los nombres de archivos y otra para los hashes
	Queue file_names;
	queueInit(&file_names, sizeof(char *));
	//read_file_names(file_names);


	//al finalizar hay que liberar la cola
	clearQueue(&file_names);
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

char * process_hash(char * file_name){
	char * command_name = "md5sum ";
	//combinar para que sea "md5sum file_name"
	char * full_command = malloc((strlen(command_name) + strlen(file_name) + 2) * sizeof(char)); 
	full_command = strcat(full_command, command_name);
	full_command = strcat(full_command, file_name);
	char * command_output = malloc(MD5LENGTH * sizeof(char)); //hay que liberarlo
	call_command(full_command, command_output);
	free(full_command);
	return command_output;
}

void ask_for_more_files(Queue q){
	//avisarle al padre
	//read_file_names(q);
}
