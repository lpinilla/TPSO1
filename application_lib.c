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

void initialize_shared_memory(void ** shm_ptr, int n_of_files){
    sem_t semaphore;
    shm_info info = (shm_info) malloc(sizeof(t_shm_info));
    if(info == NULL){
        perror("Malloc error");
        unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }
    info->last_elem_ptr = shm_ptr + sizeof(void *);
    info->semaphore = semaphore;
    //inicializando el semáforo con valor 1
    sem_init(&semaphore, 0, 1);
    info->mem_size = (n_of_files + 1) * sizeof(void *);
    shm_ptr[0] = info;
}

void clear_shared_memory(void ** shm_ptr, int n_of_files){
    //munmap(shm_ptr, ((shm_info) shm_ptr[0])->mem_size);
    sem_destroy( &( (shm_info) shm_ptr[0])->semaphore);
    free(shm_ptr[0]);
    for(int i = 1; i < n_of_files; i++){
        free(shm_ptr[i]);
    }
    shm_unlink(SHM_NAME);
}

void save_buffer_to_file(void ** shm_ptr, int n_of_files){
    FILE * file = fopen("result.txt", "w+");
    for(int i = 0; i < n_of_files;i++){
        fprintf(file, "%s \n", (char *) shm_ptr[i+1]);
    }
    fclose(file);
}

