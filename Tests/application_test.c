#include "../Testing/testing_suite.h"
#include "../application.h"
#include "../utilities/utils.h"

void shared_memory_test();
void save_buffer_to_file_test();
void write_hash_to_shm_test();

int main(void){
    create_suite("Testing the Application");

    add_test(shared_memory_test);
    add_test(save_buffer_to_file_test);
    add_test(write_hash_to_shm_test);

    run_suite();
    
    clear_suite();

    return 0;
}

//probar que puede acceder y leer desde la memoria compartida
void shared_memory_test(){
    int n_of_files = 0;
    char * result = NULL, * aux = NULL;
    //creando memoria y escribiendola
    void ** mem_ptr = create_shared_memory((n_of_files + 1) * sizeof(void *));
    shm_info mem_info = initialize_shared_memory(mem_ptr, 0);
    mem_ptr[1] = (char *) "hello world!222";  //el primer lugar esta reservado      
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
        void ** ptr = (void **) mmap(NULL, (n_of_files + 1) * sizeof(void *), PROT_READ, MAP_SHARED, shmid, 0);
        //decirle al padre lo que leyó
        write(fd[1], ptr, sizeof(char *));
        close(fd[1]);
        
        if( munmap(ptr, (n_of_files + 1) * sizeof(void *)) == -1){
            clear_shared_memory(mem_ptr, 1, mem_info);
            perror("Error dettaching memory");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    waitpid(pid, NULL, 0);
    read(fd[0], &result, sizeof(char *));
    //cerrar el fd y desvincular la memoria
    close(fd[0]);
    aux = (char *) mem_ptr[0];
    clear_shared_memory(mem_ptr, (n_of_files + 1), mem_info);
    //comparar lo que escribí con lo que leyó el hijo
    assert_equals(aux,result, sizeof(char *));
}

void save_buffer_to_file_test(){
    void ** strings = (void ** )malloc(sizeof(void *) * 3);
    int ret = 0;
    char cmd_ret = 0;
    char buff[256];
    strings[0] = "";
    strings[1] = "hello";
    strings[2] = "world!";
    int n_of_files = 2;
    save_buffer_to_file(strings, n_of_files);
    FILE * file = fopen("result.txt", "r+");
    fscanf(file, "%s", buff);
    fscanf(file, "%s", buff + 6);
    ret = strcmp(buff, strings[1]);
    ret += strcmp(buff + 6, strings[2]);
    //liberar memoria
    free(strings);
    //borrar archivo
    call_command("rm -f result.txt", &cmd_ret);
    assert_true(!ret);
}

void write_hash_to_shm_test(){
    int n_of_files = 1, aux = 0;
    void ** shm_ptr = create_shared_memory(n_of_files * sizeof(void *));
    shm_info mem_info = initialize_shared_memory(shm_ptr, n_of_files);
    char * buff = malloc(256 * sizeof(char));
    if(buff == NULL){
        perror("Malloc error");
        exit(EXIT_FAILURE);
    }
    memset(buff, 0, 256 * sizeof(char)); //limpiar todo el buffer
    call_command("md5sum ../Sistemas_Operativos_TP1_Q1_2019.pdf", buff);
    write_hash_to_shm(mem_info, buff);
    //checkear si lo que hay en memoria es lo mismo que buff
    aux = strcmp((char *) *(shm_ptr + sizeof(void *)), buff);
    free(buff);
    clear_shared_memory(shm_ptr, n_of_files, mem_info);
    assert_true(!aux);
}
