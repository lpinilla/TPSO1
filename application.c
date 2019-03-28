#include "application.h"

int main(){
    //imprimir el pid para vision
    printf("%d", getpid());

    void ** shm_ptr = create_shared_memory(10 * sizeof(void *));
    initialize_shared_memory(shm_ptr, 10);
    
    //desvincularse a la memoria y liberarla
    clear_shared_memory(shm_ptr, 0);
}

