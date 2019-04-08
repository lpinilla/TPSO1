#include "slave.h"

int main(void){
    int standars[2];
    standars[0] = STDIN_FILENO;
    standars[1] = STDOUT_FILENO;
    char  initial;
    read(STDIN_FILENO, &initial, sizeof(initial));
    int i;
    //El hijo entra en ciclo hasta que el padre le indique que cierre
    for(i=0; i<initial;i++){
        char * msg = read_pipe(standars);
        if(msg!= NULL){
            if(*msg == 0){
                free(msg);
                exit(0);
            } else {
                load_file(msg, standars);
            }
            free(msg);
        }
    }
    while(1){
        char * msg = read_pipe(standars);
        if(msg != NULL){
            if(*msg == 0){
                free(msg);
                exit(0);
            }else{
                load_file(msg, standars);
            }
            free(msg);
        }
    } 
}