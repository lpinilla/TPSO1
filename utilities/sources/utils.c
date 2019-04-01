#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include "../utils.h"
#include <string.h>
#include "../../application.h"

//utils.c , comandos auxiliares que puede usar cualquier programa

void call_command(char * command, char * buffer){
    if(command == NULL || buffer == NULL){
        return;
    }
    FILE * fd;
    fd = popen(command, "r");
    int i = 0;
    char c;
    while ( (c=getc(fd)) != '\n' && c!=EOF ){
        buffer[i] = c;
        i++;
    }
    buffer[i] = '\n';
    buffer[++i] = '\0';
    pclose(fd);
    return;
}

off_t inline calculate_size(int n_of_files){ //hacer macro
    return (n_of_files * ((MAX_FILE_NAME + HASH_LENGTH + AUX_HASH_CHARS) * sizeof(char)) + sizeof(t_shm_info));
}

