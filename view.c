#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h> 
#include <sys/stat.h> /* For mode constants */ 
#include <semaphore.h>
#include <unistd.h>



//opens or creates an existing shared memory object.
//returns a nonnegative file descriptor
int open_shm(const char *name, int oflag, mode_t mode);

//creates a new mapping in the virtual address space of the calling process
//returns a pointer to the mapped area
void *new_mapping(void *addr, size_t length, int prot, int flags,int fd, off_t offset);

//creates a new semaphore or opens an existin semaphore
//returns the address of the new or existing semaphore
sem_t *open_sem(const char *name, int oflag, mode_t mode, unsigned int value);

void print_hashes(char* hash_start,sem_t *sem);
void print_hash(char *hash_ptr);





int main(int argc,char **argv){
    //int fd_shm = open_shm(SHM, O_RDONLY | O_CREAT, S_IRWXU); // S_IRWXU is equivalent to ‘(S_IRUSR | S_IWUSR | S_IXUSR)’.
    //char *ptr_shm = mapping_shm(NULL,size_of shm,PROT_READ | PROT_WRITE, MAP_SHARED,fd_shm,0); //no entiendo los parametros :(
    //sem_t *sem_adress= open_sem(SEM, O_CREAT, S_IRWXU, 0); //ver parametros

    //shm_unlink(SHM);

    //sem_post(SEM);

       
}


void print_hashes(char* hash_start,sem_t *sem){
    //NULL fin de hashes totales

    char* hash_ptr=hash_start;
    //esto es lo que dijo ariel de no hacer y yo lo estoy haciendo :)
    while (hash_start!=NULL) {
		if (sem_wait(sem)) {
			printf("error\n");
			printf("%s\n", strerror(errno));
			exit(0);//error
		}
		print_hash(hash_ptr);

        hash_ptr+=sizeof(char*);
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
		exit(0); //exit fail?
	}
	return fd_shm;
}

void *mapping_shm(void *addr, size_t length, int prot, int flags,int fd, off_t offset){
    void *ptr_shm=mmap(NULL,length,prot,flags,fd,offset); //retorna pointer a la nueva map area
    if(ptr_shm == (void*)-1){
        printf("Error\n");
        printf("%s\n", strerror(errno));
		exit(0); //exit fail?

    }
    return ptr_shm;

}

sem_t *open_sem(const char *name, int oflag, mode_t mode, unsigned int value){
	sem_t *sem_adress = sem_open(name, oflag, mode, value);
	if (sem_adress == SEM_FAILED) {
		printf("Error\n");
		printf("%s\n", strerror(errno));
		exit(0);//exit fail?
	}
	return sem_adress;
}






