#include "testing_suite.h"

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
//faltan tests

int main(void){
   //creando un test_suite
    test_suite my_suite = create_suite(10, "Testing the Framework");

    add_test(my_suite, test1);
    add_test(my_suite, test2);
    add_test(my_suite, test3);
    add_test(my_suite, test4);
    add_test(my_suite, test5);
    add_test(my_suite, test6);
    add_test(my_suite, test7);
    add_test(my_suite, test8);
    add_test(my_suite, test9);
    add_test(my_suite, test10);

    run_suite(my_suite);
    
    free(my_suite->fun_ptrs);
    free(my_suite);
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