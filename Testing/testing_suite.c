#include "testing_suite.h"

//estaría bueno que el usuario no tenga que poner la cantidad de tests y que la calcule
test_suite create_suite(int n_of_tests, char * suite_name){
    test_suite my_suite = (t_test_suite *) malloc(sizeof(t_test_suite));
    my_suite->n_of_tests = n_of_tests;
    my_suite->suite_name = suite_name;
    my_suite->fun_index = 0;
    //creando el espacio para los tests
    my_suite->fun_ptrs = (void (**) ()) malloc(sizeof(void *) * my_suite->n_of_tests);
    return my_suite;
}

void add_test(test_suite suite , void (* fun) ()){
    if(suite == NULL){
        return;
    }
    suite->fun_ptrs[suite->fun_index] = fun;
    suite->fun_index++;
}

void run_suite(test_suite suite){
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
                printf("%d: %s \n",i, "PASS");
            }else{
                suite->suite_state = FAILURE;
                printf("%d: %s \n",i, "FAIL");
            }
        }else{ //el proceso no terminó
            suite->suite_state = FAILURE;
            printf("%d: %s \n",i, "FAIL");
        }
    }

    //TODO: hay que ver qué pasa si un proceso se cuelga

    suite->suite_state? printf("The Suite failed \n") : printf("The Suite passed \n"); 
    printf("----------------------------------------------\n");
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