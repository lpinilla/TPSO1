#include "../Testing/testing_suite.h"
#include "../application.h"
#include "../utilities/utils.h"
#include "../view.h"

void app_vision_integration_test();

int main(void){
    create_suite("Testing the Application & Vision Integration");

    add_test(app_vision_integration_test);

    run_suite();
    
    clear_suite();

    return 0;
}


void app_vision_integration_test(){
    //como si estuvieramos arrancando application
    printf("%d \n", getpid());
    int n_of_files = 1, cpid = 0, mypid = getpid();
    void ** shm_ptr = create_shared_memory(n_of_files * sizeof(void *));
    shm_info mem_info = initialize_shared_memory(shm_ptr, n_of_files);
    //agarramos un hash llamando a md5sum
    char * buff = malloc(256 * sizeof(char));
    if(buff == NULL){
        perror("Malloc error");
        exit(EXIT_FAILURE);
    }
    memset(buff, 0, 256 * sizeof(char)); //limpiar todo el buffer
    call_command("md5sum ../Sistemas_Operativos_TP1_Q1_2019.pdf", buff);
    write_hash_to_shm(mem_info, buff);
    
    //imprimir hash desde la view
    cpid = fork();
    if(cpid < 0){
        perror("Fork error");
        exit(EXIT_FAILURE);
    }else if(cpid == 0){ //hijo
        char * const str = (char * const) malloc(4 * sizeof(char));
        sprintf(str, "%d", mypid);
        char * argv[] = {"../view.so", str, NULL};
        if( execv("../view.so", argv) == -1){
            perror("Excec error");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    waitpid(cpid, NULL, 0);
    clear_shared_memory(shm_ptr, n_of_files, mem_info);
    assert_true(1); //cambiar
}