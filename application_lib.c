#include "application.h"



void * create_shared_memory(off_t size){
    void * shm_ptr = NULL;
    //crear la memoria compartida
    int shmid = 0;    
    shmid = shm_open(SHM_NAME, O_RDWR | O_CREAT, S_IRWXU);
    if(shmid < 0){
        perror("smh_open");
        exit(EXIT_FAILURE);
    }
    //definir el tamaño de la memoria (más el bloque inicial)
    if( ftruncate(shmid, size) == -1 ){ //que el tamaño sea múltiplo de una pag?
        perror("can't truncate");
        shm_unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    } 
    
    if( (shm_ptr = (void *) mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0)) == MAP_FAILED){
        //no se puede conectar con la memoria
        perror("can't attach memory");
        shm_unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }
    return shm_ptr;
}

shm_info initialize_shared_memory(void * shm_ptr, int n_of_files){
    t_shm_info shm_info;
    shm_info.last_elem_ptr = (char *) shm_ptr;
    shm_info.mem_size = calculate_size(n_of_files);
    //inicializando el semáforo con valor 1
    if (sem_init(&shm_info.semaphore, 1, 1 ) < 0 ){
        perror("Error initializing semaphore");
        unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }
    memcpy(shm_ptr, &shm_info, sizeof(t_shm_info));    
    return shm_ptr;
}

void clear_shared_memory(void * shm_ptr, int n_of_files, shm_info mem_info){  
    sem_destroy(&mem_info->semaphore);
    munmap(shm_ptr, mem_info->mem_size);
    shm_unlink(SHM_NAME);
}

void save_buffer_to_file(void * shm_ptr, int n_of_files){ //cambiar
    FILE * file = fopen("result.txt", "w+");
    for(int i = 0; i < n_of_files;i++){
        //fprintf(file, "%s \n", shm_ptr[i+1]);
    }
    fclose(file);
}

void write_hash_to_shm(void * shm_ptr, shm_info mem_info, char * hash){    
    if( sem_wait(&mem_info->semaphore) < 0){
        perror("Error in wait");
        exit(EXIT_FAILURE);
    }
    //saber cuanto desplazarse
    if(shm_ptr == mem_info->last_elem_ptr){
        mem_info->last_elem_ptr += sizeof(t_shm_info);
    }else{
        mem_info->last_elem_ptr += HASH_NAME_SIZE; //cambiar a tamaño posta
    }
    //escribir
    strcpy(mem_info->last_elem_ptr, hash);
    
    if( sem_post(&mem_info->semaphore) < 0){
        perror("Error in wait");
        exit(EXIT_FAILURE);
    }
}

