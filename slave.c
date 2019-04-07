#include "slave.h"

int main(void){
    int standars[2];
    standars[0] = stdin;
    standars[1] = stdout;
    //El hijo entra en ciclo hasta que el padre le indique que cierre
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