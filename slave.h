#ifndef _SLAVE_H_
#define _SLAVE_H_

#include "queue.h"

#define MD5LENGTH 32

//función para que lea los procesos que el padre le envió
void read_file_names(Queue q);

//función para calcular el hash md5 en base a un archivo
int process_hash(char * file_name); //creo que devuelve int

//función para que le indique al padre que puede recibir más archivos
void ask_for_more_files(Queue q);

//función para tomar el hash del output que produce el md5sum
char * grab_hash(char * md5sum_output);


#endif
