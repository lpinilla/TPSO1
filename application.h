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


#define SHM_NAME "/shm"

typedef struct{
    void * last_elem_ptr;
    sem_t semaphore;
    off_t mem_size;
}t_shm_info;

typedef t_shm_info * shm_info;

//crea una shared memory de tamaño size y conecta la memoria con el proceso
void ** create_shared_memory(off_t size);

//desvincularse de la memoria compartida
void clear_shared_memory(void ** shm_ptr);

/*función para inicializar el 1er bloque de la memoria compartida que es
**un puntero de tipo shm_info */
void initialize_shared_memory(void ** shm_ptr, int n_of_files);

//función para guardar todo el buffer a un archivo
void save_buffer_to_file(void ** shm_ptr, int n_of_files);

#endif

