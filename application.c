#include "application.h"

int main(int argc, char ** argv){
    check_app_arguments(argc);
    // creamos e inicializamos la shm
    void * shm_ptr = create_shared_memory();
    shm_info mem_info = initialize_shared_memory(shm_ptr);
    fd_set read_set;
    struct timeval tv = {10, 0};
    pipes_info pipes[NUMBER_OF_SLAVES];
    Queue * files = newQueue();
    int total_files_number = 0, aux = 0;
    // inicializamos para select
    FD_ZERO(&read_set);
    //inicializamos la queue de files
    queueInit(files, sizeof(char*));
    enqueue_args(files, argc, argv);
    aux = total_files_number = getQueueSize(files);
    if(total_files_number > SHM_MAX_FILES){
        perror("Error: Max files shm is 1000.");
        exit(EXIT_FAILURE);
    }
    //imprimir el pid para vision
    printf("%d\n", getpid());
    
    //dormir para que vision se pueda conectar
    sleep(7);   

    if(open_pipes(pipes) == -1){
        perror("Error: Pipe failed.");
        exit(EXIT_FAILURE);
    }
    fork_slaves(pipes);
    send_initial_files(files, pipes);
    send_remaining_files(aux, tv, pipes, read_set, shm_ptr, mem_info, files);
    terminate_program(mem_info, pipes, files, shm_ptr, total_files_number);
    exit(EXIT_SUCCESS);
}
