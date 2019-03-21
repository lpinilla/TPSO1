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

int main(void){
    //primero, encontrar cuantos tests hay
    int n_of_suites_found = find_tests();
    //agarrar todas las suites
    char * all_suites = fetch_all_suites(n_of_suites_found);
    //correr las suites
    run_all_suites(all_suites, n_of_suites_found);
    
    //hay que liberar cada suite
    free(all_suites);
}

char * fetch_all_suites(int n_of_suites_found){
    char * ret = (char *) malloc(MAX_FILE_NAME_LENGTH * sizeof(char) * n_of_suites_found);
    //simplemente agarrar la salida de grep
    char * buffer = malloc(MAX_FILE_NAME_LENGTH * n_of_suites_found * sizeof(char));
    call_command("ls | grep -P '[tT][eE][sS][tT][a-z]*[A-Z]*[0-9]*[_]*[a-z]*[A-Z]*(?!.)'", buffer);
                                                                                                        //agarrar los nombres de los archivos

    free(buffer);
    return ret;
}

int find_tests(){ //seguro se puede re optimizar a partir del comando anterior pero bueno
    char * buffer = (char *) malloc(sizeof(int));
    call_command("ls | grep -P '[tT][eE][sS][tT][a-z]*[A-Z]*[0-9]*[_]*[a-z]*[A-Z]*(?!.)' | wc -l", buffer);
    int ret = atoi(&buffer);
    printf("Number of files: %d \n", ret); //testing purposes
    free(buffer);
    return ret;
}

void call_command(char * command, char * buffer){
    if(command == NULL || buffer == NULL){
        return;
    }
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
        system("ls | grep -P '[tT][eE][sS][tT][a-z]*[A-Z]*[0-9]*[_]*[a-z]*[A-Z]*(?!.)' | wc -l");
        exit(EXIT_SUCCESS);
    }
    //restablecer el stdout
    fflush(stdout);
    dup2(stdout_saved, STDOUT_FILENO);
    close(stdout_saved);
    //read input
    read(fd[0], buffer, 1);
    //destruir el pipe (el extremo de lectura)
    close(fd[0]);
    return;
}

void run_all_suites(char * all_suites, int n_of_suites_found){
    //children cpid
    int cpids[n_of_suites_found];
    for(int i = 0; i < n_of_suites_found; i++){
        cpids[i] = fork();
        if(cpids[i] < 0){
            perror("Error running suite");
            exit(EXIT_FAILURE);
        }else if(cpids[i] == 0){ //el hijo
            //correr la suite
            execv(all_suites[i], NULL);
            exit(EXIT_SUCCESS);
        }        
    }
}

