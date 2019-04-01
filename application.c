#include "application.h"

#define NUMBER_OF_SLAVES 5

int main(int argc, char ** argv){

    // creamos la cola que va a contener a los files
    Queue * files = newQueue();
    queueInit(files, sizeof(char*));



    //imprimir el pid para vision
    printf("%d", getpid());
    int n_of_files = 1;
    void ** shm_ptr = create_shared_memory(n_of_files * sizeof(void *));
    shm_info mem_info = initialize_shared_memory(shm_ptr, n_of_files);
    
    printf("%p \n", mem_info); //para que me deje compilar

    // creamos los pipes de ida
    int pipes[NUMBER_OF_SLAVES][2];
    // abrimos todos los pipes de ida
    for(int i=0; i<NUMBER_OF_SLAVES; i++){
        pipe(pipes[i]);
    }
    // el padre de por si cierra su stdout
    for(int i=0; i<NUMBER_OF_SLAVES; i++){
        if(fork()==0){
            // el hijo cierra su stdin
            close(0);
            // lo redireccionamos al pipe correspondiente
            dup(pipes[i][0]);
            // para testear usamos esto desp vemos el nombre 
            execv("./slave.out", NULL);
        }
    }
    // el padre les escribe los archivos
    for(int i=0; i<NUMBER_OF_SLAVES; i++){
        write(pipes[i][1], "slave.c\n",8);
        write(pipes[i][1], "slave.h\n",8);
    }

    //desvincularse a la memoria y liberarla
    clear_shared_memory(shm_ptr, n_of_files, mem_info);
}
