#include "slave.h"
#include "utilities/utils.h"
#include "utilities/queue.h"

int main(void){
	//Una cola de los nombres de archivos
	Queue * files = newQueue();
	queueInit(files, sizeof(char *));
	char * filename;
	char * rt = calloc(sizeof(char)*100,0);
	while(1){
		read_file_names(files);
		while(getQueueSize(files)>0){
			dequeue(files,&filename);
			if(!strcmp(filename,"-1")){
				freeQueue(files);
				exit(1);
			}
			call_md5(filename,rt);
			printf("%s",rt);
		}
		// si llegamos aca es que no nos dieron senial
		// de finalizado y pedimos al padre mas archivos
		printf("-1\n");
	}
}
