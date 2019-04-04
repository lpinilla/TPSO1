#include "application.h"



void * create_shared_memory(){
    void * shm_ptr = NULL;
    //crear la memoria compartida
    int shmid = 0;    
    shmid = shm_open(SHM_NAME, O_RDWR | O_CREAT, S_IRWXU);
    if(shmid < 0){
        perror("smh_open");
        exit(EXIT_FAILURE);
    }
    //definir el tamaño de la memoria (más el bloque inicial)
    if( ftruncate(shmid, SHM_MAX_SIZE) == -1 ){ //que el tamaño sea múltiplo de una pag?
        perror("can't truncate");
        shm_unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    } 
    
    if( (shm_ptr = (void *) mmap(NULL, SHM_MAX_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0)) == MAP_FAILED){
        //no se puede conectar con la memoria
        perror("can't attach memory");
        shm_unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }
    return shm_ptr;
}

shm_info initialize_shared_memory(void * shm_ptr){
    t_shm_info shm_info;
    //apuntar a donde empiezan los hashes
    shm_info.offset = sizeof(t_shm_info); 
    shm_info.has_finished = 0;
    //inicializando el semáforo con valor 0
    if (sem_init(&shm_info.semaphore, 1, 0 ) < 0 ){
        perror("Error initializing semaphore");
        unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }
    memcpy(shm_ptr, &shm_info, sizeof(t_shm_info));    
    return shm_ptr;
}

void clear_shared_memory(void * shm_ptr, shm_info mem_info){  
    sem_destroy(&mem_info->semaphore);
    munmap(shm_ptr,SHM_MAX_SIZE);
    shm_unlink(SHM_NAME);
}

void save_buffer_to_file(void * shm_ptr, int n_of_files){
    FILE * file = fopen("result.txt", "w+");
    for(int i = 0; i < n_of_files;i++){
        fprintf(file, "%s \n", (char *) shm_ptr + i * HASH_LENGTH);
    }
    fclose(file);
}

void write_hash_to_shm(void * shm_ptr, shm_info mem_info, char * hash){   
    if( sem_post(&mem_info->semaphore) < 0){
        perror("Error in wait");
        clear_shared_memory(shm_ptr, mem_info);
        exit(EXIT_FAILURE);
    } 
    //escribir
    strcpy((char *) shm_ptr + mem_info->offset, hash);
    //desplazarse
    mem_info->offset += HASH_NAME_SIZE;
    //avisar que hay algo escrito
    
}

