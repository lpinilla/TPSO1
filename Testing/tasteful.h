#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


//Mi propio framework de testing

//              TASTEFUL
//              _, . '__ . 
//           '_(_0o),(__)o().
//         ,o(__),_)o(_)O,(__)o
//       o(_,-o(_ )(),(__(_)oO)_
//       .O(__)o,__).(_ )o(_)Oo_)
//   .----|   |   |   |   |   |_)0
//  /  .--|   |   |   |   |   |,_)
// |  /   |   |   |   |   |   |o(_)
// |  |   |   |   |   |   |   |_/`)
// |  |   |   |   |   |   |   |O_) 
// |  |   |   |   |   |   |   |
// |  \   |   |   |   |   |   |
//  \  '--|   |   |   |   |   |
//   '----|   |   |   |   |   |
//        |   |   |   |   |   |
//        \   \   \   /   /   /
//         `"""""""""""""""""`

enum STATE {SUCCESS = 0, FAILURE};

typedef struct{
    //punteros a puntero a función
    void  (** fun_ptrs) (void);
    int n_of_tests;
    enum STATE suite_state; 
    char * suite_name;
}t_test_suite;

typedef t_test_suite * test_suite;

typedef test_suite * suites; //un puntero a suites de tests

void run_suite(test_suite suite);

void assert_equals(const void * a, const void * b, size_t size_of_data);
void assert_not_equals(const void * a, const void * b, size_t size_of_data);
void assert_true(int i);
void assert_false(int i);