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
    //definir el tamaño de la memoria (más el bloque inicial)
    if( ftruncate(shmid, size) == -1 ){
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

shm_info initialize_shared_memory(void ** shm_ptr, int n_of_files){
    sem_t * semaphore = malloc(sizeof(sem_t));
    if(semaphore == NULL){
        perror("Malloc error");
        unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }
    shm_info info = (shm_info) malloc(sizeof(t_shm_info));
    if(info == NULL){
        perror("Malloc error");
        unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }
    info->last_elem_ptr = (char **) (shm_ptr + sizeof(void *));
    info->semaphore = semaphore;
    //inicializando el semáforo con valor 1
    if(sem_init(semaphore, 1, 1) < 0){
        perror("Error initializing semaphore");
        unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }
    info->mem_size = (n_of_files + 1) * sizeof(void *);
    shm_ptr[0] = info;
    return info;
}

void clear_shared_memory(void ** shm_ptr, int n_of_files, shm_info mem_info){    //aca esta el error 
    sem_destroy(mem_info->semaphore);
    for(int i = 0; i <= n_of_files; i++){
        free(*(shm_ptr + i * sizeof(void *)));
    }
    munmap(shm_ptr, mem_info->mem_size);
    shm_unlink(SHM_NAME);
}

void save_buffer_to_file(void ** shm_ptr, int n_of_files){
    FILE * file = fopen("result.txt", "w+");
    for(int i = 0; i < n_of_files;i++){
        fprintf(file, "%s \n", (char *) shm_ptr[i+1]);
    }
    fclose(file);
}

void write_hash_to_shm(shm_info mem_info, char * hash){
    if( sem_wait((mem_info->semaphore)) < 0){
        perror("Error in wait");
        exit(EXIT_FAILURE);
    }
    *(mem_info->last_elem_ptr) =  hash;
    mem_info->last_elem_ptr += sizeof(char *);
    if( sem_post((mem_info->semaphore)) < 0){
        perror("Error in wait");
        exit(EXIT_FAILURE);
    }
}

