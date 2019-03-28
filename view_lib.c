#include "view.h"

shm_info get_shm_info(int fd_shm){
	shm_info ret = NULL;    
	//agarro el primer void* de la shm, para tener una referencia a la estructura t_shm_info
	void **first_struct= (void **) mapping_shm(NULL,sizeof(void*),PROT_READ, MAP_SHARED,fd_shm,0);
	//guardo el puntero a la estructura
	ret = (shm_info) first_struct[0]; 
	//limpio memoria que ya no uso
	munmap(first_struct,sizeof(void*)); 
	return ret;
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
    void **ptr_shm= (void **) mmap(NULL,length,prot,flags,fd,offset); //retorna pointer a la nueva map area
    if(ptr_shm == (void*)-1){
        printf("Error\n");
        printf("%s\n", strerror(errno));
		exit(EXIT_FAILURE); //exit fail?

    }
    return ptr_shm;
}