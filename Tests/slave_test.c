#include "../Testing/testing_suite.h"
#include "../slave.h"

void md5_call_test(void);
void read_parent_pipe_test(void);

int main(){

    create_suite("Testing Slave");

    add_test(md5_call_test);
    add_test(read_parent_pipe_test);

    run_suite();
    
    clear_suite();

    return 0;
}

void md5_call_test(void){
    char * expected = "../Sistemas_Operativos_TP1_Q1_2019.pdf: dbbc672b0dec675712e78f98cfe88c25" ;
    int length = strlen(expected);
    char buff[length];
    call_md5("../Sistemas_Operativos_TP1_Q1_2019.pdf", buff);
    assert_equals(buff, expected, sizeof(char) * length);
}

void read_parent_pipe_test(void){
    int test_pipe[2];
    if(pipe(test_pipe)<0){
        perror("ERROR CREANDO EL PIPE");
        return;
    }
    close(test_pipe[0]);
    close(test_pipe[1]);

}
