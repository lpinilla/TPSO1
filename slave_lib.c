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
	// esto lo hago para ignorar los casos de md5sum que entran a leer
	// stdin sin parar
	char c = file_name[0];
	if(c=='-' || c=='\0' || c=='\n' || c==' ' || c=='\t' || c==EOF){
		strcpy(output, "Invalid file sintax \n");
		return;
	}
	if(access(file_name, R_OK) == -1){	
		output = strcpy(output, file_name);	
		output = strcpy(output, ": file doesnt exist \n");
		return;
	}
	char command_name[] = "md5sum ";
	int size = strlen(command_name) + strlen(file_name);
	char full_command[size+1];
	full_command[0] = '\0';
	strcat(full_command, command_name);
	strcat(full_command, file_name);
	call_command(full_command, output);
	return;
}