#include "application.h"

int main(){
    //imprimir el pid para vision
    printf("%d", getpid());

    void ** shm_ptr = create_shared_memory(10 * sizeof(void *));

    shm_ptr[0] = NULL; //para que no compile con warning
    
    
    //desvincularse a la memoria
    unlink_shared_memory();
}

