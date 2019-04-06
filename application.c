#include "application.h"

int main(int argc, char ** argv){
    // si no tenemos argumentos no hay nada que hacer
    if(argc==0 || argc == 1){
        return 1;
    }
    //imprimir el pid para vision
    printf("%d \n", getpid());
    void * shm_ptr = create_shared_memory();
    shm_info mem_info = initialize_shared_memory(shm_ptr);
    sleep(7);

    Queue * files = newQueue();
    queueInit(files, sizeof(char*));
    enqueue_args(files, argc, argv);
    int files_number = getQueueSize(files);

    // se puede borrar despues(lo uso para probar save_buffer_to_file al final)
    int aux = files_number;
    
    

    

    pipes_info pipes[NUMBER_OF_SLAVES];

    if(open_pipes(pipes) == -1){
        perror("Error: Pipe creation failed.");
        clear_shared_memory(shm_ptr, mem_info);
        exit(EXIT_FAILURE);
    }

    int i;
    int p;

    // padre crea procesos esclavos y les envia trabajo
    for(i=0; i<NUMBER_OF_SLAVES && getQueueSize(files)>0 ; i++){
        p=fork();
        if(p<0){
            perror("Error: Fork failed.");
            clear_shared_memory(shm_ptr, mem_info);
            exit(EXIT_FAILURE);
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
        for(i=0; i<NUMBER_OF_SLAVES && files_number>0; i++){
            char * hash = read_pipe(pipes[i].pipe_in);
            if(hash != NULL){
                //Imprimo el hash TEMPORALMENTE hasta ver lo de la shm
                //printf("%s\n", hash);
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
    mem_info->has_finished = 1;
    close_pipes(pipes);

    freeQueue(files);

    save_buffer_to_file(shm_ptr, aux);

    //desvincularse a la memoria y liberarla
    clear_shared_memory(shm_ptr, mem_info);
    
    return 0;
}
