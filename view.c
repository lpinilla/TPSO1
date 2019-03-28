#include "view.h"



int main(int argc, char ** argv){

	int app_pid = 0;

	if(argc < 2){
		printf("application's pid must be given");
		exit(EXIT_FAILURE);
	}else if(argc == 2){
		app_pid = atoi(argv[1]); //pasar a int el pid del padre
	}
	printf("%d \n", app_pid); //para que deje compilar

	//conectarse a la memoria
	//agarrar el file descriptor de la shared memory
	int fd_shm = open_shm(SHM_NAME, O_RDONLY | O_CREAT, S_IRWXU); // S_IRWXU is equivalent to ‘(S_IRUSR | S_IWUSR | S_IXUSR)’.

	//agarrar el bloque inicial y guardarlo en una referencia
	shm_info aux = get_shm_info(fd_shm);
	printf("Mem size: %ld\n",aux->mem_size);

	//mapeo la memoria con su actual size
    void **ptr_shm = mapping_shm(NULL,aux->mem_size,PROT_READ | PROT_WRITE, MAP_SHARED,fd_shm,sizeof(void*));
    
	print_hashes((char**) ptr_shm,&aux->semaphore,aux);

	//munmap(ptr_shm, );
    //shm_unlink(SHM);
	return 0;
       
}


