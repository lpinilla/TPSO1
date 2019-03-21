#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "tasteful.h"

void assert_equals(const void * a, const void * b, size_t size_of_data);
void assert_not_equals(const void * a, const void * b, size_t size_of_data);
void assert_true(int i);
void assert_false(int i);

void run_suite(test_suite suite);