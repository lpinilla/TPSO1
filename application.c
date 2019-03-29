#include "application.h"


int main(){
    //imprimir el pid para vision
    printf("%d", getpid());
    int n_of_files = 1;
    void ** shm_ptr = create_shared_memory(n_of_files * sizeof(void *));
    shm_info mem_info = initialize_shared_memory(shm_ptr, n_of_files);
    
    printf("%p \n", mem_info); //para que me deje compilar
    
    //desvincularse a la memoria y liberarla
    clear_shared_memory(shm_ptr, n_of_files, mem_info);
}
