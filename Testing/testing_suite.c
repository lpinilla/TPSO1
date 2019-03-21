#include "testing_suite.h"

//testing suite to test the framework

//eliminar después
void test1(void);
void test2(void);
void test3(void);
void test4(void);
void test5(void);
void test6(void);
void test7(void);
void test8(void);
void test9(void);
void test10(void);


int main(void){
    //creando un test_suite
    test_suite my_suite = (t_test_suite *) malloc(sizeof(t_test_suite));
    //creando el espacio para los tests
    int n_of_tests = 10;
    my_suite->n_of_tests = n_of_tests;
    my_suite->suite_name = "Testing the Framework";
    my_suite->fun_ptrs = (void (**) ()) malloc(sizeof(void *) * n_of_tests);
    my_suite->fun_ptrs[0] = test1;
    my_suite->fun_ptrs[1] = test2;
    my_suite->fun_ptrs[2] = test3;
    my_suite->fun_ptrs[3] = test4;
    my_suite->fun_ptrs[4] = test5;
    my_suite->fun_ptrs[5] = test6;
    my_suite->fun_ptrs[6] = test7;
    my_suite->fun_ptrs[7] = test8;
    my_suite->fun_ptrs[8] = test9;
    my_suite->fun_ptrs[9] = test10;

    run_suite(my_suite);
    free (my_suite->fun_ptrs);
    free(my_suite);
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

        //dejo esto acá por las dudas, hay que ver si estaba bien
        //acá o si hice bien en separarlo


        /*waitpid(cpid[i], &child_status, 0);
        if(WIFEXITED(child_status)){//si terminó
            if(!WEXITSTATUS(child_status)){
                printf("%d: %s \n",i, "PASS");
            }else{
                suite->suite_state = FAILURE;
                printf("%d: %s \n",i, "FAIL");
            }
        }*/
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


void test1(){
    printf("hello \n");
    return;
}

void test2(){
    printf("world! \n");
    return;
}

//todos buenos

void test3(){
    assert_true(1 == 1);
}

void test4(){
    assert_false(1 == 2);
}

void test5(){
    int a = 1;
    int b = 1;
    assert_equals(&a, &b, sizeof(int));
}

void test6(){
    int a = 1;
    int b = 2;
    assert_not_equals(&a,&b, sizeof(int));
}

//ahora los malos

void test7(){
    assert_true(1 == 0);
}

void test8(){
    assert_false(1 == 1);
}

void test9(){
    int a = 1;
    int b = 2;
    assert_equals(&a, &b, sizeof(int));
}
void test10(){
    int a = 1;
    int b = 1;
    assert_not_equals(&a,&b, sizeof(int));
}

//FALTA TESTEAR

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