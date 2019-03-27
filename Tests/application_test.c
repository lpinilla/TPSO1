#include "../Testing/testing_suite.h"
#include "../application.h"
#include <fcntl.h>

void shared_memory_test();

int main(void){
    create_suite("Testing the Application");

    add_test(shared_memory_test);

    run_suite();
    
    clear_suite();

    return 0;
}

//probar que puede acceder y leer desde la memoria compartida
void shared_memory_test(){
    char * result = NULL;
    //creando memoria y escribiendola
    void ** mem_ptr = create_shared_memory(10 * sizeof(void *));
    mem_ptr[0] = (char *) "hello world!";        
    int fd[2] = {0,1};
    if( pipe(fd) < 0){
        perror("Error on pipe");
        exit(EXIT_FAILURE);
    }
    //creando proceso hijo que acceda a esa memoria
    int pid = fork();
    if(pid < 0){
        perror("Error forking");
        exit(EXIT_FAILURE);
    }else if(pid == 0){ //proceso hijo
        //abrir la memoria compartida
        int shmid = shm_open(SHM_NAME, O_RDONLY, 0660);
        void ** ptr = (void **) mmap(NULL, 10 * sizeof(void *), PROT_READ, MAP_SHARED, shmid, 0);
        //decirle al padre lo que leyó
        write(fd[1], ptr, sizeof(char *));
        close(fd[1]);
        
        if( munmap(ptr, 10 * sizeof(void *)) == -1){
            unlink_shared_memory();
            perror("Error dettaching memory");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    waitpid(pid, NULL, 0);
    read(fd[0], &result, sizeof(char *));
    //cerrar el fd y desvincular la memoria
    close(fd[0]);
    unlink_shared_memory();
    //comparar lo que escribí con lo que leyó el hijo
    assert_equals(mem_ptr[0],result, sizeof(char *));
}