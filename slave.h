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
#include <ctype.h>

/*función para calcular el hash md5 en base a un archivo retorna 'hash nombredel archivo'*/
int call_md5(char * file_name, char * output);
//recibe el mensaje con el archivo y la pipe de entrada para enviar al padre
void load_file(char * file_name, int pipe[2]);

#endif
