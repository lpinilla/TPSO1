#include "../Testing/testing_suite.h"
#include "../application.h"
#include "../view.h"

void get_shm_info_test();
void arguments_check_test();

int main(void){
    create_suite("Testing the View");

    add_test(get_shm_info_test);
    add_test(arguments_check_test);

    run_suite();
    
    clear_suite();

    return 0;
}

void get_shm_info_test(){
    int n_of_files = 0, cpid = 0, fds[2] = {0,1};
    void ** shm_ptr = create_shared_memory((n_of_files + 1) * sizeof(void *));
    shm_info aux = NULL, aux2 = NULL;
    shm_info mem_info = initialize_shared_memory(shm_ptr, n_of_files);
    aux2 = shm_ptr[0];
    if(pipe(fds) < 0){
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if(cpid < 0){
        perror("fork error");
        exit(EXIT_FAILURE);
    }else if(cpid == 0){
        int fd = shm_open(SHM_NAME, O_RDONLY | O_CREAT, S_IRWXU);
        shm_info target = get_shm_info(fd);
        write(fds[1], &target, sizeof(shm_info));
        close(fds[1]);
        shm_unlink(SHM_NAME);
        exit(EXIT_SUCCESS);
    }
    read(fds[0], &aux, sizeof(shm_info));
    clear_shared_memory(shm_ptr, n_of_files, mem_info);
    assert_equals(aux2, aux, sizeof(t_shm_info));
}

void arguments_check_test(){
    int argc = 2;
    char * const argv = {"../view.so", "1234"};
    check_arguments(argc, argv);
}