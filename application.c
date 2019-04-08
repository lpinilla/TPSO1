#include "application.h"

int main(int argc, char ** argv){

    // si no tenemos argumentos no hay nada que hacer
    if(argc==0 || argc == 1){
        return 1;
    }

    void * shm_ptr;
    shm_info mem_info;

    fd_set read_set;
    struct timeval tv;
    pipes_info pipes[NUMBER_OF_SLAVES];

    Queue * files;
    int total_files_number, actual_files, i;

    // creamos e inicializamos la shm
    shm_ptr = create_shared_memory();
    mem_info = initialize_shared_memory(shm_ptr);

    // inicializamos para select
    FD_ZERO(&read_set);

    // creamos e inicializamos la queue de files
    files = newQueue();
    queueInit(files, sizeof(char*));
    enqueue_args(files, argc, argv);

    total_files_number = getQueueSize(files);
    actual_files = total_files_number;

    
    //sleep(7);    

    //imprimir el pid para vision
    printf("%d\n", getpid());

    if(open_pipes(pipes) == -1){
        perror("Error: Pipe failed.");
        exit(EXIT_FAILURE);
    }

    fork_slaves(pipes);

    send_initial_files(files, pipes);

    while(actual_files>0){
        tv.tv_sec=10;
        tv.tv_usec=0;
        for(i=0; i<NUMBER_OF_SLAVES;i++){
            FD_SET(pipes[i].pipe_in[0],&read_set);
        }
        //el primer argumento de select debe ser el pipe mas grande de todos + 1, aclarado en libreria de select
        int maxfd = pipes[0].pipe_in[1];
        for(i=1; i<NUMBER_OF_SLAVES;i++){
            if(maxfd < pipes[i].pipe_in[1]){
                maxfd = pipes[i].pipe_in[1];
            }
        }
        if(select(maxfd+1, &read_set, NULL, NULL, &tv) < 0){
            perror("Error: Select failed.\n");
            exit(EXIT_FAILURE);
        }
        for(i=0; i<NUMBER_OF_SLAVES && actual_files>0; i++){
            if(FD_ISSET(pipes[i].pipe_in[0],&read_set)){
                char * hash = read_pipe(pipes[i].pipe_in);
                if(hash != NULL && strcmp(hash,"-1")!=0){
                    actual_files--;
                    write_hash_to_shm(shm_ptr, mem_info, hash);
                }
                else if(getQueueSize(files)>0 && strcmp(hash,"-1")==0){
                    send_file(files,pipes[i].pipe_out);
                }
                free(hash);
            }
        }
    }

    mem_info->has_finished = 1;

    close_pipes(pipes);

    freeQueue(files);

    save_buffer_to_file(shm_ptr, total_files_number);

    //desvincularse a la memoria y liberarla
    clear_shared_memory(shm_ptr, mem_info);
    
    return 0;
}
