#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h> 
#include <sys/stat.h> /* For mode constants */ 
#include <semaphore.h>
#include <unistd.h>
#include "application.h" //ver si hace falta importar




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





int main(int argc, char ** argv){

	int app_pid = 0;

	if(argc < 2){
		printf("application's pid must be given");
		exit(EXIT_FAILURE);
	}else if(argc == 2){
		app_pid = atoi(argv[1]); //pasar a int el pid del padre
	}
	printf("%d \n", app_pid); //para que deje compilar
	//agarro puntero de donde arranca a shm
    int fd_shm = open_shm(SHM_NAME, O_RDONLY | O_CREAT, S_IRWXU); // S_IRWXU is equivalent to ‘(S_IRUSR | S_IWUSR | S_IXUSR)’.

	//agarro el primer void* de la shm, para saber el size total de la shm
	void **first_struct=mapping_shm(NULL,sizeof(void*),PROT_READ | PROT_WRITE, MAP_SHARED,fd_shm,0);
	shm_info aux = (shm_info) first_struct[0];	//mapeo la memoria
	munmap(first_struct,sizeof(void*)); //limpio memoria que ya no uso

	printf("Mem size: %ld\n",aux->mem_size);

	//mapeo la memoria con su actual size
    void **ptr_shm = mapping_shm(NULL,aux->mem_size,PROT_READ | PROT_WRITE, MAP_SHARED,fd_shm,sizeof(void*));
    
	print_hashes((char**) ptr_shm,&aux->semaphore,aux);


    //shm_unlink(SHM);
	//sem_post(SEM);
	return 0;
       
}


void print_hashes(char** hash_start,sem_t *sem,shm_info aux){
    char** hash_ptr=hash_start;

    //esto consultar con Ariel:)
	int cant_files=((aux->mem_size)/sizeof(void*))-1;
    for(int i=0;i<cant_files;i++){
		if (sem_wait(sem)) {
			printf("error\n");
			printf("%s\n", strerror(errno));
			exit(EXIT_FAILURE);//error
		}
		print_hash(hash_ptr[i]);
		sem_post(sem);
        hash_ptr+=sizeof(void*);
	}
	
}

void print_hash(char *hash_ptr){
    printf("%s\n",hash_ptr);

}

int open_shm(const char *name, int oflag, mode_t mode){
    int fd_shm = shm_open(name, oflag, mode); 
	
	if (fd_shm == -1) {
		printf("Error\n");
		printf("%s\n", strerror(errno));
		exit(EXIT_FAILURE); //exit fail?
	}
	return fd_shm;
}

void **mapping_shm(void *addr, size_t length, int prot, int flags,int fd, off_t offset){
    void **ptr_shm=mmap(NULL,length,prot,flags,fd,offset); //retorna pointer a la nueva map area
    if(ptr_shm == (void*)-1){
        printf("Error\n");
        printf("%s\n", strerror(errno));
		exit(EXIT_FAILURE); //exit fail?

    }
    return ptr_shm;

}

