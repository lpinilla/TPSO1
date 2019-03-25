#include "../Testing/testing_suite.h"

void test1(void);

int main(){

    create_suite("Testing Slave");

    add_test(test1);

    run_suite();
    
    clear_suite();

    return 0;
}

void test1(void){
    assert_true(1==1);
}