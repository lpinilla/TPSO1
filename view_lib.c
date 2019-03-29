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


void print_hashes(char** hash_start, shm_info mem_info){
	printf("asdaaaaaaaaaaaaa \n");
    char** hash_ptr=hash_start;
    //esto consultar con Ariel:)
	int cant_files=((mem_info->mem_size)/sizeof(void*))-1;
    for(int i=0;i<cant_files;i++){
		if (sem_wait(mem_info->semaphore)) {
			printf("error\n");
			printf("%s\n", strerror(errno));
			exit(EXIT_FAILURE);//error
		}
		print_hash(hash_ptr[i]);
		if( sem_post(mem_info->semaphore)){
			printf("error\n");
			printf("%s\n", strerror(errno));
			exit(EXIT_FAILURE);//error
		}
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

void ** connect_to_shm(shm_info * mem_info){
	//agarrar el file descriptor de la shared memory
	int fd_shm = open_shm(SHM_NAME, O_RDONLY | O_CREAT, S_IRWXU); // S_IRWXU is equivalent to ‘(S_IRUSR | S_IWUSR | S_IXUSR)’.
	//agarrar el bloque inicial y guardarlo en una referencia
	*mem_info = get_shm_info(fd_shm);
	printf("%p \n", *mem_info);	
	printf("Mem size: %ld\n", (*mem_info)->mem_size);
	printf("imprimimos \n");
	//mapeo la memoria con su actual size
    void **ptr_shm = mapping_shm(NULL,(*mem_info)->mem_size,PROT_READ | PROT_WRITE, MAP_SHARED,fd_shm,sizeof(void*));
	return ptr_shm;
}

void mem_disconnect(void ** ptr_shm, shm_info mem_info){
	munmap(ptr_shm, mem_info->mem_size);
    shm_unlink(SHM_NAME);
}

void check_arguments(int argc, char ** argv){
	int app_pid = 0;
	if(argc < 2){
		printf("application's pid must be given");
		exit(EXIT_FAILURE);
	}else{
		app_pid = atoi(argv[1]); //pasar a int el pid del padre
	}
	printf("app pid's %d \n", app_pid); //para que deje compilar
}