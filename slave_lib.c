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
	//combinar para que sea "md5sum file_name"
	char * full_command = (char *) malloc((strlen(command_name) + strlen(file_name) + 1) * sizeof(char));  //malloc puede fallar
	if(full_command == NULL){
		printf("Malloc failed, not enough memory");
		return NULL;
	}
	full_command = strcat(full_command, command_name);
	full_command = strcat(full_command, file_name);
    //hay que liberarlo
	char * command_output = (char *) malloc(MD5LENGTH * sizeof(char)); 
	if(command_output == NULL){
		printf("Malloc failed, not enough memory");
		free(full_command);
		return NULL;
	}
	call_command(full_command, command_output);
	free(full_command);
	return command_output;
}

void ask_for_more_files(Queue q){
	//avisarle al padre
	//read_file_names(q);
}