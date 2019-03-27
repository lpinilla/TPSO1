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

//crea una shared memory de tamaño size y conecta la memoria con el proceso
void ** create_shared_memory(off_t size);

//desvincularse de la memoria compartida
void unlink_shared_memory();

typedef struct{
    void * last_elem_ptr;
    sem_t semaphore;
}t_shm_info;

typedef t_shm_info * shm_info;



#endif
