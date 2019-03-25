#include "../Testing/testing_suite.h"

void test1(void);
void test2(void);
void test3(void);
void test4(void);
void test5(void);
void test6(void);
//faltan tests

int main(void){
   //creando un test_suite
    test_suite my_suite = create_suite(6, "Testing the Framework");

    add_test(my_suite, test1);
    add_test(my_suite, test2);
    add_test(my_suite, test3);
    add_test(my_suite, test4);
    add_test(my_suite, test5);
    add_test(my_suite, test6);

    run_suite(my_suite);
    
    free(my_suite->fun_ptrs);
    free(my_suite);
}

//cambiar los 1eros 2 tests

void test1(){
    assert_true(1 == 1);
}

void test2(){
    assert_false(1 == 0);
}

void test3(){
    assert_true('a' == 'a');
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

//FALTA TESTEAR