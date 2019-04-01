#include "view.h"

void print_hashes(void * hash_start, shm_info mem_info, int n_of_files){	//arreglar
    for(int i=0; i<n_of_files; i++){
		if (sem_wait(&mem_info->semaphore) < 0) {			
			printf("error\n");
			printf("%s\n", strerror(errno));
			exit(EXIT_FAILURE);//error
		}
		printf("%s\n", (char *) hash_start + mem_info->offset);
		if( sem_post(&mem_info->semaphore)){
			printf("error\n");
			printf("%s\n", strerror(errno));
			exit(EXIT_FAILURE);//error
		}
	}
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

void * mapping_shm(void *addr, size_t length, int prot, int flags,int fd, off_t offset){
    void *ptr_shm= mmap(addr,length,prot,flags,fd,offset); //retorna pointer a la nueva map area
    if(ptr_shm == (void*)-1){
        printf("Error\n");
        printf("%s\n", strerror(errno));
		exit(EXIT_FAILURE); //exit fail?
    }	
    return ptr_shm;
}

void * connect_to_shm(shm_info * mem_info, off_t size){
	//agarrar el file descriptor de la shared memory
	int fd_shm = open_shm(SHM_NAME, O_RDWR, S_IRWXU); // S_IRWXU is equivalent to ‘(S_IRUSR | S_IWUSR | S_IXUSR)’.
	//mapeo la memoria con su actual size
    void * ptr_shm = mapping_shm(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,fd_shm,0);
	*mem_info = (shm_info) ptr_shm;	
	return ptr_shm;
}

void mem_disconnect(void * ptr_shm, shm_info mem_info){
	if( munmap(ptr_shm, mem_info->mem_size) == -1){ //sacar el hardcodeo
		perror("munmap");
		exit(EXIT_FAILURE);
	}
    //shm_unlink(SHM_NAME);
	//close?
}

void check_arguments(int argc, char ** argv){
	int app_pid = 0;
	if(argc < 2){
		printf("application's pid must be given");
		exit(EXIT_FAILURE);
	}else{
		app_pid = atoi(argv[1]); //pasar a int el pid del padre
	}
	app_pid++; //para que deje compilar
}