#include "ush.h"

char *get_pwd(void) //memory allocation func
{
    char *pwd;
    char *ptr;
    int32_t path_max = pathconf(".", _PC_PATH_MAX);
    size_t size = (path_max == -1) ? 1024 : 
           (path_max > 10240) ? 10240 : path_max;

    for(pwd = ptr = NULL; ptr == NULL; size *= 2) {
        if((pwd = realloc(pwd, size + 1)) == NULL)
            strerror(errno);

        ptr = getcwd(pwd, size);
 
        if(ptr == NULL && errno != ERANGE)
            strerror(errno);
    }

    return pwd;
}

void pwd_func(t_ush *ush, t_process *process)
{
    // char *pwd = get_pwd();
    // add_newline(&pwd);
    // calloc_args(ush->result, 2);
    // write_arg(ush->result, pwd);
    process->status = 0;
}
