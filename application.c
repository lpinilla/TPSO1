#include "application.h"
#include <dirent.h> 
#include <stdio.h> 
#include <string.h>

int main(int argc, char ** argv){
    // si no tenemos argumentos no hay nada que hacer
    if(argc==0 || argc == 1){
        return 1;
    }

    Queue * files = newQueue();
    queueInit(files, sizeof(char*));
    enqueue_args(files, argc, argv);
    int files_number = getQueueSize(files);
    
    //imprimir el pid para vision
    //printf("%d", getpid());
    //int n_of_files = 1;
    void * shm_ptr = create_shared_memory();
    shm_info mem_info = initialize_shared_memory(shm_ptr);
    //printf("%p \n", mem_info); //para que me deje compilar

    pipes_type pipes[NUMBER_OF_SLAVES];

    // abrimos todos los pipes
    for(int i=0; i<NUMBER_OF_SLAVES; i++){
        if(pipe(pipes[i].pipe_out)==-1){
            fprintf(stderr, "Error: Pipe creation failed." );
            return 1;
        }
        //fcntl(pipes[i].pipe_out[0], F_SETFL, O_NONBLOCK);
        if(pipe(pipes[i].pipe_in)==-1){
            fprintf(stderr, "Error: Pipe creation failed." );
            return 1;
        }
       // fcntl(pipes[i].pipe_in[0], F_SETFL, O_NONBLOCK);
    }

    int p;

    // el padre de por si cierra su stdout
    for(int i=0; i<NUMBER_OF_SLAVES && getQueueSize(files)>0 ; i++){
        p=fork();
        if(p<0){
            fprintf(stderr, "Error: Fork failed." );
            return 1;
        }

        //Proceso hijo/esclavo
        else if(p==0){
            //Cerramos el final de escritura del pipe de salida
            close(pipes[i].pipe_out[1]);
            //Cerramos el final de lectura del pipe de entrada
            close(pipes[i].pipe_in[0]);

            //El hijo entra en ciclo hasta que el padre le indique que cierre
            while(1){
                char * msg = read_pipe(pipes[i].pipe_out);
                if(msg != NULL){
                    if(*msg == 0){
                        close(pipes[i].pipe_out[0]);
                        close(pipes[i].pipe_in[1]);
                        exit(0);
                    }else{
                        load_file(msg, pipes[i].pipe_in);
                    }
                    free(msg);
                }
            }
        }

        //Proceso padre
        else{
            //Cerramos el final de lectura del pipe de salida
            close(pipes[i].pipe_out[0]);
            //Cerramos el final de escritura del pipe de entrada
            close(pipes[i].pipe_in[1]);
            //Envia un archivo a cada hijo
            send_file(files, pipes[i].pipe_out);
        }
    }

    // el padre recibe los hashes, los guarda en la shm y si quedan archivos por enviar a esclavos los envia
    while(files_number>0){
        for(int i=0; i<NUMBER_OF_SLAVES && files_number>0; i++){
            char * hash = read_pipe(pipes[i].pipe_in);
            if(hash != NULL){

                //Imprimo el hash TEMPORALMENTE hasta ver lo de la shm
                printf("%s\n", hash);
                //------

                files_number--;

                //Guarda hash en la shm
                write_hash_to_shm(shm_ptr, mem_info, hash);

                if(getQueueSize(files)>0){
                    send_file(files, pipes[i].pipe_out);
                }
                free(hash);
            }
        }
    }

    //Proceso padre envia un 0 por los pipes a los hijos para indicarles que terminen su proceso
    char exit_msg = 0;
    for(int i=0; i<NUMBER_OF_SLAVES;i++){
        write(pipes[i].pipe_out[1], &exit_msg, 1);

        //Cerramos el final de lectura del pipe de entrada
        close(pipes[i].pipe_in[0]);
        //Cerramos el final de escritura del pipe de salida
        close(pipes[i].pipe_out[1]);
    }

    freeQueue(files);

    //desvincularse a la memoria y liberarla
    clear_shared_memory(shm_ptr, mem_info);

}
