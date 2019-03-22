#ifndef _TASTEFUL_H_
#define _TASTEFUL_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

//tamaño maximo de nombre de los archivos de test
#define MAX_FILE_NAME_LENGTH 16



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
    //path del suite
    char * suite_name;
    /*índice del array (podría usarlo a futuro para que sea más cómodo
    **agregar funciones)*/
    int fun_index;
}t_test_suite;

typedef t_test_suite * test_suite;

void run_all_suites(char ** all_suites, int n_of_suites_found);

/*devuelve un array dinámico de suites en base a los archivos
**binarios cuyos nombres empiecen con test (o sus variaciones)*/
char ** fetch_all_suites(int n_of_suites_found);

/*devuelve la cantidad de archivos binarios cuyos nombres
**comienzan con test o alguna variación de este*/
int find_tests();

/*Llama al comando command por la shell de linux y guarda el
**resultado en el buffer provisto*/
void call_command(char * command, char * buffer);

//hacer todos los free correspondientes
void free_space(int n_of_suites_found, char ** suites);

//regex original: grep -P '[tT][eE][sS][tT][a-z]*[A-Z]*[0-9]*[_]*[a-z]*[A-Z]*(?!.)'

#endif

