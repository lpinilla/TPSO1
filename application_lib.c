#include "application.h"

void ** create_shared_memory(off_t size){
    void ** shm_ptr = NULL;
    //crear la memoria compartida
    int shmid = 0;
    shmid = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0660); //hacer un define con la constante 0660
    if(shmid < 0){
        perror("smh_open");
        exit(EXIT_FAILURE);
    }
    //definir el tamaño de la memoria
    if( ftruncate(shmid, size) == -1 ){ // TAMAÑO TEMPORAL
        perror("can't truncate");
        shm_unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    } 
    
    if( (shm_ptr = (void **) mmap(NULL, size, PROT_WRITE, MAP_SHARED, shmid, 0)) == MAP_FAILED){
        //no se puede conectar con la memoria
        perror("can't attach memory");
        shm_unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }
    return shm_ptr;
}

void unlink_shared_memory(){
    shm_unlink(SHM_NAME);
}
