#ifndef _TESTING_SUITE_H_
#define _TESTING_SUITE_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "tasteful.h"

#define ARRAY_STEP 5

//los asserts posibles que se pueden hacer
void assert_equals(const void * a, const void * b, size_t size_of_data);
void assert_not_equals(const void * a, const void * b, size_t size_of_data);
void assert_true(int i);
void assert_false(int i);

//correr todas los tests del suite
void run_suite();

//función para inicializar la suite
void create_suite(char * suite_name);

//función para agregar un test al suite
void add_test(void (* fun) ());

//función que se encarga de hacer todos los free
void clear_suite();

test_suite suite;

#endif
