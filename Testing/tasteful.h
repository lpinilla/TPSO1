#ifndef _TASTEFUL_H_
#define _TASTEFUL_H_

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

enum STATE {SUCCESS = 0, FAILURE}; //ver si hay que crear un 3er estado default

typedef struct{
    //punteros a puntero a función
    void  (** fun_ptrs) (void);
    //cantidad de tests que contiene
    int n_of_tests;
    //el estado final una vez corridos los tests (por defecto SUCCESS)
    enum STATE suite_state; 
    //nombre de la suite
    char * suite_name;
}t_test_suite;

typedef t_test_suite * test_suite;

//un puntero a suites de tests
typedef test_suite * suites; 


void run_all_suites(test_suite *);

#endif
