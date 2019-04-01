#include "view.h"

int main(int argc, char ** argv){
	check_arguments(argc, argv);	
	shm_info mem_info = NULL;
	//conectarse a la memoria
	int n_of_files = 1; 	//cambiar, esto lo va a recibir del pipe
	void * ptr_shm = connect_to_shm(&mem_info, calculate_size(n_of_files)); //corregir el tamaño
	//printf("EN VIEW: %s \n", (char *) (ptr_shm + sizeof(HASH_NAME_SIZE)));
	//print_hashes(ptr_shm, mem_info, 1);


	mem_disconnect(ptr_shm, mem_info);
	return 0;
}


