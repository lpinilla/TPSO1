#include "slave.h"

#define PATH_LEN 256
#define MD5_LEN 32

#define STR_VALUE(val) #val
#define STR(name) STR_VALUE(name)

int call_md5(char *file_name, char *output)
{
    #define MD5SUM_CMD_FMT "md5sum %." STR(PATH_LEN) "s 2>/dev/null"
    char cmd[PATH_LEN + sizeof (MD5SUM_CMD_FMT)];
    sprintf(cmd, MD5SUM_CMD_FMT, file_name);
    #undef MD5SUM_CMD_FMT

	sprintf(output, "%s: ", file_name);

    FILE *p = popen(cmd, "r");
    if (p == NULL) return 0;

    int aux = strlen(output);
    int i, ch;
    for (i = aux; i < (MD5_LEN + aux) && isxdigit(ch = fgetc(p)); i++) {
        output[i] = ch;
    }

    output[i] = '\0';
    pclose(p);

    return i == MD5_LEN;
}