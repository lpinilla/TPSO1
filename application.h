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


#define SHM_NAME "/shm"

typedef struct{
    char ** last_elem_ptr;
    sem_t * semaphore;
    off_t mem_size;
}t_shm_info;

typedef t_shm_info * shm_info;

//crea una shared memory de tamaño size y conecta la memoria con el proceso
void ** create_shared_memory(off_t size);

//desvincularse de la memoria compartida y limpiar todos los punteros
void clear_shared_memory(void ** shm_ptr, int n_of_files, shm_info mem_info);

/*función para inicializar el 1er bloque de la memoria compartida que es
**un puntero de tipo shm_info */
shm_info initialize_shared_memory(void ** shm_ptr, int n_of_files);

//función para guardar todo el buffer a un archivo
void save_buffer_to_file(void ** shm_ptr, int n_of_files);

//función para guardar un hash en la memoria compartida, utiliza semáforo
void write_hash_to_shm(shm_info mem_info, char * hash);

#endif

