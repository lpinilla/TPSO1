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

/*void enqueue_dir(Queue * files, char * path)
{
  DIR * d = opendir(path); // open the path
  if(d==NULL) return; // if was not able return
  struct dirent * dir; // for the directory entries
  while ((dir = readdir(d)) != NULL) // if we were able to read somehting from the directory
    {
        char d_path[strlen(path) + strlen(dir->d_name) + 2];
        sprintf(d_path, "%s/%s", path, dir->d_name);
      if(dir-> d_type != DT_DIR){ // if the type is not directory just print it with blue
        enqueue(files, &d_path); // recall with the new path
      }
      else if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 ) // if it is a directory
      {
        enqueue_dir(files, d_path); // recall with the new path
      }
    }
    closedir(d); // finally close the directory
}*/

void enqueue_args(Queue * files, int argc, char ** argv){
    // creamos la estructura stat
    struct stat path_stat;
    // creamos la cola que va a contener a los files
    for(int i=1; i<argc; i++){
        char * filename = malloc(sizeof(char)*strlen(argv[i]));
        strcpy(filename,argv[i]);
        // la inicializamos por cada argumento
        stat(filename, &path_stat);
        // usamos la macro para ver si es una file
        if(S_ISREG(path_stat.st_mode)){
            // si es una file es facil, la encolamos
            enqueue(files ,&filename);
        }
        // usamos otra macro para ver si es un directorio
        else if(S_ISDIR(path_stat.st_mode)){
            /*char fname[strlen(filename) + 1];
            strcpy(fname, filename);
            free(filename);
            enqueue_dir(files, fname);*/

            // si es un dir vemos todas las files adentro
            DIR * dir;
            struct dirent * ent;
            dir = opendir(filename);
            while((ent = readdir(dir)) != NULL){
                char * name = malloc(sizeof(char)*strlen(ent->d_name)
                +sizeof(char)*strlen(filename));

                // aca armamos el path relativo
                strcpy(name,filename);
                strcat(name,"/");
                strcat(name,ent->d_name);

                stat(name, &path_stat);
                if(S_ISREG(path_stat.st_mode))
                    enqueue(files, &name);
            }
        }
        else{
            // si no es directorio o file nos vamos
            exit(1);
        }
    }
}

void send_file(Queue * files, int pipe[2]){
    char * file_name;
    dequeue(files, &file_name);
    write(pipe[1], file_name, strlen(file_name)+1);
}

void load_file(char * file_name, int pipe[2]){  
    char hash_msg[HASH_LENGTH + strlen(file_name) + 3];
    call_md5(file_name, hash_msg);
    write(pipe[1], hash_msg, strlen(hash_msg)+1);
}

char * read_pipe(int pipe[2]){
    int index = 0;
    int size = BLOCK;
    char * msg = malloc(BLOCK);
    char buf;
    if(!(read(pipe[0], &buf, 1) > 0)){
        free(msg);
        return NULL;
    }

    msg[index++]=buf;

    while(buf != 0){
        if(read(pipe[0], &buf, 1) > 0){
            if(index +1 == size){
                msg = realloc(msg, size + BLOCK);
                size += BLOCK;
            }
            msg[index++] = buf;
        }
    }

    msg = realloc(msg, index+2);
    msg[index] = 0;
    return msg;
}