#include "application.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include "utilities/queue.h"

#define NUMBER_OF_SLAVES 5


int main(int argc, char ** argv){
    // si no tenemos argumentos no hay nada que hacer
    if(argc==1){
        return 1;
    }
    // creamos la estructura stat
    struct stat path_stat;
    // creamos la cola que va a contener a los files
    Queue * files = newQueue();
    queueInit(files, sizeof(char*));
    for(int i=1; i<argc; i++){
        char * filename = malloc(sizeof(char*)*strlen(argv[i]));
        strcpy(filename,argv[i]);
        // la inicializamos por cada argumento
        stat(filename, &path_stat);
        // usamos la macro para ver si es una file
        if(S_ISREG(path_stat.st_mode)){
            // si es una file es facil, la encolamos
            enqueue(files ,&filename);
            printf("ERA UNA FILE \n");
        }
        // usamos otra macro para ver si es un directorio
        else if(S_ISDIR(path_stat.st_mode)){
            // si es un dir vemos todas las files adentro
            DIR * dir;
            struct dirent * ent;
            dir = opendir(argv[i]);
            while((ent = readdir(dir)) != NULL){
                struct stat path_stat_inside;
                stat(ent->d_name,&path_stat_inside);
                if(S_ISREG(path_stat_inside.st_mode))
                    enqueue(files, &(ent->d_name));
            }
            printf("ERA UN DIR \n");
        } 
        else{
            // si no es directorio o file nos vamos
            exit(1);
        }
    }
    char  * aux;
    while(getQueueSize(files)!=0){
        dequeue(files, &aux);
        printf("%s\n", aux);
    }


    /*

    //imprimir el pid para vision
    printf("%d", getpid());
    int n_of_files = 1;
    void ** shm_ptr = create_shared_memory(n_of_files * sizeof(void *));
    shm_info mem_info = initialize_shared_memory(shm_ptr, n_of_files);
    
    printf("%p \n", mem_info); //para que me deje compilar

    // creamos los fd para los pipes de ida
    int pipes[NUMBER_OF_SLAVES][2];
    // abrimos todos los pipes de ida
    for(int i=0; i<NUMBER_OF_SLAVES; i++){
        pipe(pipes[i]);
    }
    // el padre de por si cierra su stdout
    for(int i=0; i<NUMBER_OF_SLAVES; i++){
        if(fork()==0){
            // el hijo cierra su stdin
            close(0);
            // lo redireccionamos al pipe correspondiente
            dup(pipes[i][0]);
            // para testear usamos esto desp vemos el nombre 
            execv("./slave.out", NULL);
        }
    }
    // el padre les escribe los archivos
    for(int i=0; i<NUMBER_OF_SLAVES; i++){
        write(pipes[i][1], "slave.c\n",8);
        write(pipes[i][1], "slave.h\n",8);
    }

    //desvincularse a la memoria y liberarla
    clear_shared_memory(shm_ptr, n_of_files, mem_info);

    */
}
