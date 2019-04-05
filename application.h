#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <string.h>
#include <dirent.h>
#include "utilities/queue.h"
#include "utilities/utils.h"
#include "slave.h"

#define SHM_NAME "/shm"
#define NUMBER_OF_SLAVES 5
#define BLOCK 10
#define HASH_LENGTH 32

typedef struct{
  int pipe_out[2];
  int pipe_in[2];
} pipes_type;

typedef struct{
    //offset al último elemento agregado
    size_t offset, has_finished;
    sem_t semaphore;
}t_shm_info;

typedef t_shm_info * shm_info;

//crea una shared memory de tamaño SHM_MAX_SIZE y conecta la memoria con el proceso
void * create_shared_memory();

//desvincularse de la memoria compartida y limpiar todos los punteros
void clear_shared_memory(void * shm_ptr, shm_info mem_info);

/*función para inicializar el 1er bloque de la memoria compartida que es
**un puntero de tipo shm_info */
shm_info initialize_shared_memory(void * shm_ptr);

//función para guardar todo el buffer a un archivo
void save_buffer_to_file(void * shm_ptr, int n_of_files);

//función para guardar un hash en la memoria compartida, utiliza semáforo
void write_hash_to_shm(void * shm_ptr, shm_info mem_info, char * hash);

//función auxiliar para calcular el tamaño total de la memoria
//no la necesitamos mas
//off_t calculate_size(int n_of_files);

//arma la queue de argumentos del proceso padre
void enqueue_args(Queue * files, int argc, char ** argv);

//envia un archivo al pipe pasado
void send_file(Queue * files, int pipe_out[2]);

//Recibe el mensaje con el archivo y la pipe de entrada para enviar al padre
void load_file(char * file_name, int pipe[2]);

//Lee un mensaje del pipe hasta encontrar un caracter 0 y lo retorna
char * read_pipe(int pipe[2]);


#endif

