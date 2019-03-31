#ifndef _SLAVE_H_
#define _SLAVE_H_

//librerías propias
#include "utilities/queue.h"
#include "utilities/utils.h"
//librerías de C
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define MD5LENGTH 32

//función para que lea los procesos que el padre le envió
void read_file_names(Queue q);

/*función para calcular el hash md5 en base a un archivo
**retorna 'hash nombredel archivo'*/

void call_md5(char * file_name, char * output);

//función para que le indique al padre que puede recibir más archivos
void ask_for_more_files(Queue q);



#endif
