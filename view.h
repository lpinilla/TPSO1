#ifndef _VIEW_H_
#define _VIEW_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h> 
#include <sys/stat.h> /* For mode constants */ 
#include <semaphore.h>
#include <unistd.h>
#include "application.h"


//opens or creates an existing shared memory object.
//returns a nonnegative file descriptor
int open_shm(const char *name, int oflag, mode_t mode);

//creates a new mapping in the virtual address space of the calling process
//returns a pointer to the mapped area
void **mapping_shm(void *addr, size_t length, int prot, int flags,int fd, off_t offset);

//creates a new semaphore or opens an existin semaphore
//returns the address of the new or existing semaphore
sem_t *open_sem(const char *name, int oflag, mode_t mode, unsigned int value);

void print_hashes(char** hash_start,sem_t *sem,shm_info str);

void print_hash(char *hash_ptr);

//función para guardar una referncia a la estructura en el primer puntero
//de la memoria compartida
shm_info get_shm_info(int fd_shm);

#endif
