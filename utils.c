#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
//utils.c , comandos auxiliares que puede usar cualquier programa

void call_command(char * command, char * buffer){
    if(command == NULL || buffer == NULL){
        return;
    }
    //reroutear el stdout para agarrar el resultado del proceso hijo
    int fd[2];
    int cpid = 0, stdout_saved;
    stdout_saved = dup(STDOUT_FILENO);
    if(pipe(fd) < 0){
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }
    //cerramos stdout y lo conectamos con el pipe
    close(STDOUT_FILENO);
    dup(fd[1]);
    close(fd[1]);
    //creamos el proceso hijo
    cpid = fork();
    if( cpid < 0){
        perror("Error creating child process");
        exit(EXIT_FAILURE);
    }else if(cpid == 0){ //proceso hijo
        system(command);
        exit(EXIT_SUCCESS);
    }
    //restablecer el stdout
    fflush(stdout);
    dup2(stdout_saved, STDOUT_FILENO);
    close(stdout_saved);
    //read input
    while( (read(fd[0], buffer, 1) > 0)){
        if(*buffer == '\n'){
            *buffer = '|';
        }
        buffer++;
        
    }
    
    //destruir el pipe (el extremo de lectura)
    close(fd[0]);
    return;
}