#include "testing_suite.h"


void create_suite(char * suite_name){
    suite = (t_test_suite *) malloc(sizeof(t_test_suite));
    //suite->n_of_tests = n_of_tests;
    suite->suite_name = suite_name;
    suite->fun_index = 0;
    suite->fun_ptrs = NULL;
    //creando el espacio para los tests
}

void add_test(void (* fun) ()){
    if(suite == NULL){
        return;
    }
    if((suite->fun_index % ARRAY_STEP) == 0){
        suite->fun_ptrs = (void (**)()) realloc((void (**)()) suite->fun_ptrs,
                         sizeof(void *) * ARRAY_STEP * (1 + suite->fun_index));
    }
    suite->fun_ptrs[suite->fun_index] = fun;
    suite->fun_index++;
    suite->n_of_tests++;
}

void run_suite(){
    printf("----------------------------------------------\n");
    printf("Testing Suite \" %s \" \n", suite->suite_name);
    printf("----------------------------------------------\n");
    int cpid[suite->n_of_tests];    
    int child_status = 0;
    suite->suite_state = SUCCESS;

    //correr los procesos
    for(int i = 0; i < suite->n_of_tests; i++){
        cpid[i] = fork();
        child_status = 0;
        if(cpid[i] == -1){
            perror("Error creating child process");
            exit(EXIT_FAILURE);
        }else if(cpid[i] == 0){ //proceso hijo, el test
            suite->fun_ptrs[i]();
            exit(EXIT_SUCCESS);
        }
    }
    /*Esperar a que los procesos terminen, lo separo porque si
    **esta todo junto, obliga a que los tests se hagan secuencialmente
    **lo que puede hacer que un test lleve mucho tiempo de I/O por ejemplo
    **pero no usa el CPU y mientras tanto otro test podría ir corriendo*/

    for(int i = 0; i < suite->n_of_tests; i++){
        waitpid(cpid[i], &child_status, 0);
        if(WIFEXITED(child_status)){//si terminó
            if(!WEXITSTATUS(child_status)){
                printf("\033[0;32m");
                printf("%d: %s \n",i, "PASS");
                printf("\033[0m");
            }else{
                suite->suite_state = FAILURE;
                printf("\033[0;31m");
                printf("%d: %s \n",i, "FAIL");
                printf("\033[0m");
            }
        }else{ //el proceso no terminó
            suite->suite_state = FAILURE;
            printf("\033[0;31m");
            printf("%d: %s \n",i, "FAIL");
            printf("\033[0m");
        }
    }

    //TODO: hay que ver qué pasa si un proceso se cuelga
    if(suite->suite_state){
        printf("\033[0;31m");
        printf("\t \t \t The Suite failed \n");
        printf("\033[0m");
    }else{
        printf("\033[0;32m");
        printf("\t \t \t The Suite passed \n");
        printf("\033[0m");
    }
    printf("----------------------------------------------\n");
}

void clear_suite(){
    free(suite->fun_ptrs);
    free(suite);
}


void assert_equals(const void * a, const void * b, size_t size_of_data){
    if((a == NULL) && (b == NULL)) {
        exit(EXIT_SUCCESS);
    }
    if( ((a == NULL) && (b != NULL)) || 
        ((a != NULL) && (b == NULL)) ) {
        exit(EXIT_FAILURE);
    }
    memcmp(a,b,size_of_data)? exit(EXIT_FAILURE) : exit(EXIT_SUCCESS);
}

void assert_not_equals(const void * a, const void * b, size_t size_of_data){
    if((a == NULL) && (b == NULL)) {
        exit(EXIT_FAILURE);
    }
    if( ((a == NULL) && (b != NULL)) || 
        ((a != NULL) && (b == NULL)) ) {
        exit(EXIT_SUCCESS);
    }
    memcmp(a,b,size_of_data)? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
}

inline void assert_true(int i){ //podría ser una macro
    i? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
}

inline void assert_false(int i){ //podría ser una macro
    i? exit(EXIT_FAILURE) : exit(EXIT_SUCCESS);
}