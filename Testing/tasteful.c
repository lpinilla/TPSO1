#include "tasteful.h"
#include "testing_suite.h"


//Mi propio framework de testing

//              TASTEFUL
//              _, . '__ . 
//           '_(_0o),(__)o().
//         ,o(__),_)o(_)O,(__)o
//       o(_,-o(_ )(),(__(_)oO)_
//       .O(__)o,__).(_ )o(_)Oo_)
//   .----|   |   |   |   |   |_)0
//  /  .--|   |   |   |   |   |,_)
// |  /   |   |   |   |   |   |o(_)
// |  |   |   |   |   |   |   |_/`)
// |  |   |   |   |   |   |   |O_) 
// |  |   |   |   |   |   |   |
// |  \   |   |   |   |   |   |
//  \  '--|   |   |   |   |   |
//   '----|   |   |   |   |   |
//        |   |   |   |   |   |
//        \   \   \   /   /   /
//         `"""""""""""""""""`

suites fetch_all_suites();
int find_tests();

int main(void){
    //primero, encontrar cuantos tests hay: ls | grep 'test' | wc -l
    
    //agarrar todas las suites
    //suites all_suites = fetch_all_suites();
    find_tests();
    
    //correr las suites


    //liberar espacios
    //free(all_suites);
}

suites fetch_all_suites(){
    int n_of_suites_found = find_tests(); //ver como armar esto
    suites ret = (suites) malloc(sizeof(test_suite) * n_of_suites_found);

    return ret;
}

int find_tests(){
    //reroutear el stdout para agarrar el resultado del proceso hijo
    int fd[2];
    int cpid = 0, stdout_saved;
    stdout_saved = dup(STDOUT_FILENO);
    if(pipe(fd) < 0){
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }
    //cerramos stdout y lo conectamos con el pipe
    close(STDOUT_FILENO);
    dup(fd[1]);
    close(fd[1]);
    //creamos el proceso hijo
    cpid = fork();
    if( cpid < 0){
        perror("Error creating child process");
        exit(EXIT_FAILURE);
    }else if(cpid == 0){ //proceso hijo
        system("ls | grep 'test' | wc -l");
        exit(EXIT_SUCCESS);
    }
    //restablecer el stdout
    fflush(stdout);
    dup2(stdout_saved, STDOUT_FILENO);
    close(stdout_saved);
    //read input
    char buff = 0;
    read(fd[0], &buff, 1);
    int ret = atoi(&buff);
    printf("Number of files: %d \n", ret);
    //destruir el pipe (el extremo de lectura)
    close(fd[0]);
    return ret;
}
