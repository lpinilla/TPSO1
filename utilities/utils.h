#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "../application.h"

#define MAX_FILE_NAME 256
#define HASH_LENGTH 32
#define AUX_HASH_CHARS 2
#define HASH_NAME_SIZE ((MAX_FILE_NAME + HASH_LENGTH + AUX_HASH_CHARS) * sizeof(char))
#define SHM_MAX_SIZE HASH_NAME_SIZE*1000

//función para llamar a un comando de linux y guardar el resultado en un buffer
void call_command(char * command, char * buffer);

//función auxiliar para calcular el tamaño de la memoria basado en la cantidad de archivos
off_t calculate_size(int n_of_files);

#endif
