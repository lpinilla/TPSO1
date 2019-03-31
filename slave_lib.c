#include "slave.h"
#include "utilities/utils.h"

//leer los nombres de los archivos de stdin
void read_file_names(Queue * q){
	char * line = NULL;
	ssize_t linecap = 0;
	size_t linelen = 0;
	linelen = getline(&line, (size_t *) &linecap, stdin);
	line[linelen-1] = '\0';
	enqueue(q, &line);
}

void call_md5(char * file_name, char * output){
	char command_name[] = "md5sum ";
	int size = strlen(command_name) + strlen(file_name);
	char full_command[size+1];
	full_command[0] = '\0';
	strcat(full_command, command_name);
	strcat(full_command, file_name);
	call_command(full_command, output);
	return;
}