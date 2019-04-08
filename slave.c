#include "slave.h"

int main(void){
    int standards[2] = {STDIN_FILENO, STDOUT_FILENO};
    char initial, * msg = NULL;
    read(STDIN_FILENO, &initial, sizeof(char));
    read_initial_files(initial, msg, standards); 
    read_remaining_files(initial, msg, standards);
}