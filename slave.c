#include "slave.h"
#include "utilities/utils.h"
#include "utilities/queue.h"

int main(void){
	//Una cola de los nombres de archivos y otra para los hashes
	Queue * files = newQueue();
	queueInit(files, sizeof(char *));
	char * filename;
	read_file_names(files);
	dequeue(files,&filename);
	printf("\n %s",filename);
	//call_md5("a.out",rt);
	//al finalizar hay que liberar la cola
	freeQueue(files);
}
