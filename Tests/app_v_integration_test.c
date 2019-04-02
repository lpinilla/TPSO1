#include "../Testing/testing_suite.h"
#include "../application.h"
#include "../utilities/utils.h"
#include "../view.h"

void app_vision_integration_test();
void ejemplo_profe();

int main(void){
    create_suite("Testing the Application & Vision Integration");

    add_test(app_vision_integration_test);
    //add_test(ejemplo_profe);

    run_suite();
    
    clear_suite();

    return 0;
}


void app_vision_integration_test(){
    //como si estuvieramos arrancando application
    int n_of_files = 1, cpid = 0, mypid = getpid(), child_status = 0, fd[2], stdout_saved = dup(STDOUT_FILENO);
    void * shm_ptr = create_shared_memory(calculate_size(n_of_files));
    char buffer[HASH_NAME_SIZE];
    memset(buffer, 0, HASH_NAME_SIZE);
    shm_info mem_info = initialize_shared_memory(shm_ptr, n_of_files);
    if(pipe(fd) < 0){
      perror("pipe");
      exit(EXIT_FAILURE);
    }
    //agarramos un hash llamando a md5sum
    char * buff = (char *) malloc(256 * sizeof(char));
    if(buff == NULL){
        perror("Malloc error");
        exit(EXIT_FAILURE);
    }
    memset(buff, 0, 256 * sizeof(char)); //limpiar todo el buffer
    call_command("md5sum ../Sistemas_Operativos_TP1_Q1_2019.pdf", buff);
    write_hash_to_shm(shm_ptr, mem_info, buff);
    mem_info->has_finished = 1; //se terminaron los archivos

    //cerrando stdout
    close(STDOUT_FILENO);
    dup(fd[1]);
    close(fd[1]);

    //imprimir hash desde la view
    cpid = fork();
    if(cpid < 0){
        perror("Fork error");
        exit(EXIT_FAILURE);
    }else if(cpid == 0){ //hijo
        char * const str = (char * const) malloc(5 * sizeof(char));
        sprintf(str, "%d", mypid);
        char * argv[] = {"../view.so", str, NULL};
        if( execv("../view.so", argv) == -1){
            perror("Excec error");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    waitpid(cpid, &child_status, 0);
    //reestablecer stdout
    fflush(stdout);
    dup2(stdout_saved, STDOUT_FILENO);
    close(stdout_saved);
    //leer el printf
    read(fd[0], buffer, HASH_NAME_SIZE);
    close(fd[0]);
    clear_shared_memory(shm_ptr, n_of_files, mem_info);
    //check_child_status(child_status);
    assert_true(!strncmp(buffer,
                        "dbbc672b0dec675712e78f98cfe88c25  ../Sistemas_Operativos_TP1_Q1_2019.pdf\n",
                        HASH_NAME_SIZE));
}

void ejemplo_profe(){
    int fd = shm_open("/SHM_NAME", O_CREAT | O_RDWR, S_IRWXU), cpid = 0;
  ftruncate(fd, 100 * sizeof(char));
  void *shm_ptr = mmap(NULL, 100 * sizeof(char), PROT_WRITE, MAP_SHARED, fd, 0);
  printf("parent ptr %p \n", shm_ptr);
  cpid = fork();

  if (cpid < 0) {
    perror("fork error");
    exit(EXIT_FAILURE);
  } else if (cpid == 0) {  // proceso hijo
    int fd2 = shm_open("/SHM_NAME", O_RDONLY, S_IRWXU);
    void *shm_ptr2 = mmap(NULL, 100 * sizeof(char), PROT_READ, MAP_SHARED, fd2, 0);
    printf("child ptr %p \n", shm_ptr2);
    // cada 1 segundo el child lee el valor del primer byte de la memoria
    char i;
    char *p = (char *) shm_ptr2;
    for (i = 0; i < 3; i++){
      printf("Child read: %d\n", *p);
      sleep(1);
    }
    munmap(shm_ptr2, 100 * sizeof(char *));
    // shm_unlink("/SHM_NAME");
    exit(EXIT_SUCCESS);
  } else{
    //cada 1 segundo parent escribe un nuevo valor al inicio de la memoria
    char i;
    char *p = (char *) shm_ptr;
    for (i = 0; i < 3; i++){
      *p = i;
      sleep(1);
    }
  }

  waitpid(cpid, NULL, 0);
  munmap(shm_ptr, 100 * sizeof(char *));
  shm_unlink("/SHM_NAME");
}

//escribir un archivo, esperar y escribir otro mientras vision lee
void write_and_read_continuous_test(){

}