#include "slave.h"
#include "utilities/utils.h"

//leer los nombres de los archivos de stdin
void read_file_names(Queue q){
	char * line = NULL;
	ssize_t linecap = 0;
	size_t linelen;
	while( (linelen = getline(&line, (size_t *) &linecap, stdin)) > 0){ 
		enqueue(&q, &line);
	}
}

void call_md5(char * file_name, char * output){
	static char command_name[] = "md5sum ";
	int size = strlen(command_name) + strlen(file_name);
	char full_command[size];
	strcat(full_command, command_name);
	strcat(full_command, file_name);
	call_command(full_command, output);
	return;
}

void ask_for_more_files(Queue q){
	//avisarle al padre
	//read_file_names(q);
}