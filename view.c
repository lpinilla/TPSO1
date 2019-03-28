#include "view.h"



int main(int argc, char ** argv){

	check_arguments(argc, argv);

	//conectarse a la memoria
	shm_info mem_info = NULL;
	void ** ptr_shm = connect_to_shm(&mem_info);
    
	//print_hashes((char**) ptr_shm, mem_info);

	mem_disconnect(ptr_shm, mem_info);
	return 0;
}


