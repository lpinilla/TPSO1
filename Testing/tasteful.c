#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


//My propio framework de testing

enum STATE {SUCCESS = 0, FAILURE};

typedef struct{
    //punteros a puntero a función
    void  (** fun_ptrs) (void);
    int n_of_tests;
    enum STATE suite_state; 
}t_test_suite;

typedef t_test_suite * test_suite;

typedef test_suite * suites; //un puntero a suites de tests

void run_suite(test_suite suite);

void assert_equals(const void * a, const void * b, size_t size_of_data);
void assert_not_equals(const void * a, const void * b, size_t size_of_data);
void assert_true(int i);
void assert_false(int i);

//eliminar después
void test1(void);
void test2(void);
void test3(void);
void test4(void);
void test5(void);
void test6(void);


int main(void){
    //creando un test_suite
    test_suite my_suite = (t_test_suite *) malloc(sizeof(t_test_suite));
    //creando el espacio para los tests
    int n_of_tests = 2;
    my_suite->n_of_tests = n_of_tests;
    my_suite->fun_ptrs = (void (**) ()) malloc(sizeof(void *) * n_of_tests);
    my_suite->fun_ptrs[0] = test1;
    my_suite->fun_ptrs[1] = test2;
    my_suite->fun_ptrs[2] = test3;
    my_suite->fun_ptrs[3] = test4;
    my_suite->fun_ptrs[4] = test5;
    my_suite->fun_ptrs[5] = test6;

    run_suite(my_suite);


    free (my_suite->fun_ptrs);
    free(my_suite);
}

void run_suite(test_suite suite){
    int cpid[suite->n_of_tests];    
    for(int i = 0; i < suite->n_of_tests; i++){
        cpid[i] = fork();

        if(cpid[i] == -1){
            perror("Error creating child process");
            exit(EXIT_FAILURE);
        }else if(cpid[i] == 0){ //proceso hijo, el test
            suite->fun_ptrs[i]();

            //aca debería haber algo para que se checkee si estaba bien o mal

            //terminar de correr el hijo
            exit(EXIT_SUCCESS);
        }
        //si algún hijo devolvió algo mal, indicar que hay una falla en el suite
        //suite->suite_state = FAILURE;
        //printf("Un error en el test %d \n", i);
    }
    suite->suite_state = SUCCESS;
}

void test1(){
    printf("hello \n");
    return;
}

void test2(){
    printf("world! \n");
    return;
}

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

void assert_true(int i){
    i? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
}

void assert_false(int i){
    i? exit(EXIT_FAILURE) : exit(EXIT_SUCCESS);
}
