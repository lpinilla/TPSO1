#include "../Testing/testing_suite.h"
#include "../application.h"
#include "../utilities/utils.h"

void shared_memory_test();
void save_buffer_to_file_test();
void write_hash_to_shm_test();

int main(void){
    create_suite("Testing the Application");

    //add_test(shared_memory_test);
    add_test(save_buffer_to_file_test);
    add_test(write_hash_to_shm_test);

    run_suite();
    
    clear_suite();

    return 0;
}

//probar que puede acceder y leer desde la memoria compartida
void shared_memory_test(){
    int n_of_files = 1;
    char * result = NULL, * aux = NULL, * string = (char *) malloc(13 * sizeof(char));
    //creando memoria y escribiendola
    void * mem_ptr = create_shared_memory(n_of_files * sizeof(char *) + sizeof(t_shm_info));
    shm_info mem_info = initialize_shared_memory(mem_ptr, n_of_files);
    strcpy(string, "hello world!");    
     *((char **) mem_ptr + sizeof(t_shm_info)) = (char *) string;
    int fd[2] = {0,1};
    if( pipe(fd) < 0){
        perror("Error on pipe");
        exit(EXIT_FAILURE);
    }
    //creando proceso hijo que acceda a esa memoria
    int pid = fork();
    if(pid < 0){
        perror("Error forking");
        exit(EXIT_FAILURE);
    }else if(pid == 0){ //proceso hijo
        //abrir la memoria compartida
        int shmid = shm_open(SHM_NAME, O_RDONLY, 0660);
        void * ptr = mmap(NULL, calculate_size(n_of_files), PROT_READ, MAP_SHARED, shmid, 0);
        //decirle al padre lo que leyó
        write(fd[1], ((char **) ptr + sizeof(t_shm_info)), sizeof(char *));
        close(fd[1]);
        
        if( munmap(ptr,calculate_size(n_of_files)) == -1){
            clear_shared_memory(mem_ptr, n_of_files, mem_info);
            perror("Error dettaching memory");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    waitpid(pid, NULL, 0);
    read(fd[0], &result, sizeof(char *));
    //cerrar el fd y desvincular la memoria
    close(fd[0]);
    aux = *((char **) mem_ptr + sizeof(t_shm_info));
    clear_shared_memory(mem_ptr, n_of_files, mem_info);
    if(aux == NULL || result == NULL){
        exit(EXIT_FAILURE);
    }
    //comparar lo que escribí con lo que leyó el hijo
    assert_equals(aux,result, sizeof(char *));
}

void save_buffer_to_file_test(){
    int n_of_files = 2, ret = 0;
    void * strings = (void *) malloc(2 * HASH_LENGTH);
    char cmd_ret = 0, buff[HASH_LENGTH * 2];
    memset(strings, 0, 2 * HASH_LENGTH);
    strcpy(strings, "hello");
    strcpy(strings + HASH_LENGTH, "world!");
    save_buffer_to_file(strings, n_of_files);
    //leer del archivo
    FILE * file = fopen("result.txt", "r+");
    fscanf(file, "%s", buff);
    fscanf(file, "%s", buff + HASH_LENGTH);
    ret = strncmp(buff, strings, HASH_LENGTH);
    ret += strcmp(buff + HASH_LENGTH, strings + HASH_LENGTH);
    //liberar memoria
    free(strings);
    //borrar archivo
    call_command("rm -f result.txt", &cmd_ret);
    assert_true(!ret);
}

void write_hash_to_shm_test(){
    int n_of_files = 1, aux = 0;
    void * shm_ptr = create_shared_memory(calculate_size(n_of_files));
    shm_info mem_info = initialize_shared_memory(shm_ptr, n_of_files);
    char * buff = (char *) malloc(256 * sizeof(char));
    if(buff == NULL){
        perror("Malloc error");
        exit(EXIT_FAILURE);
    }
    memset(buff, 0, 256 * sizeof(char)); //limpiar todo el buffer
    call_command("md5sum ../Sistemas_Operativos_TP1_Q1_2019.pdf", buff);
    write_hash_to_shm(shm_ptr, mem_info, buff);
    //checkear si lo que hay en memoria es lo mismo que buff
    aux = strncmp(buff, shm_ptr + sizeof(t_shm_info), HASH_NAME_SIZE);
    free(buff);
    clear_shared_memory(shm_ptr, n_of_files, mem_info);
    assert_true(!aux);
}
