#include "slave.h"
#include "utilities/utils.h"
#include "utilities/queue.h"

int main(void){
	//Una cola de los nombres de archivos y otra para los hashes
	Queue files;
	queueInit(&files, (size_t)sizeof(char *));
	char rt[100];
	call_md5("slave.c",rt);
	printf("%s", rt);
	//al finalizar hay que liberar la cola
	clearQueue(&files);
}
