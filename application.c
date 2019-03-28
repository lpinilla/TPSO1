#include "application.h"


int main(){
    //imprimir el pid para vision
    printf("%d", getpid());

    void ** shm_ptr = create_shared_memory(10 * sizeof(void *));
    shm_info mem_info = initialize_shared_memory(shm_ptr, 10);
    
    printf("%p \n", mem_info); //para que me deje compilar
    
    //desvincularse a la memoria y liberarla
    clear_shared_memory(shm_ptr, 0, mem_info);
}
