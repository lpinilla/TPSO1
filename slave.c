#include "slave.h"
#include "utilities/utils.h"
#include "utilities/queue.h"

int main(void){
	//Una cola de los nombres de archivos y otra para los hashes
	Queue * files = newQueue();
	queueInit(files, sizeof(char *));
	char * filename;
	char rt[100];
	while(1){
		read_file_names(files);
		while(getQueueSize(files)>0){
			dequeue(files,&filename);
			if(!strcmp(filename,"-1"))
				exit(1);
			call_md5(filename,rt);
			printf("%s",rt);
		}
	}
	//al finalizar hay que liberar la cola
	freeQueue(files);
}
