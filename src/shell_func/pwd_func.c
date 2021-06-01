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

/* 
-L Use PWD from environment, even if it contains symbolic links
-P Avoid all symbolic links
*/

void pwd_func(t_ush *ush, t_process *process)
{
    uint32_t options = get_opt(process->args, "LP");
    uint8_t opt_L = READ_OPT(options, 1);
    uint8_t opt_P = READ_OPT(options, 2);

    char *pwd = getcwd(NULL, 0);
    char **path = NULL;

    if (process->args[1] != NULL && !strcmp(process->args[1], "-P")) {
        write(1, pwd, strlen(pwd));
        write(1, "\n", 1);
        mx_strdel(&pwd);
        process->status = 0;
        return;
    }

    path = mx_strsplit(pwd, '/');

    if (*path == NULL)
        mx_printstr("/");
    else {
        for (int i = 0; path[i]; i++) {
            mx_printstr("/");
            mx_printstr(path[i]);
        }
    }
    
    write(1, "\n", 1);
    mx_del_strarr(&path);
    mx_strdel(&pwd);

    process->status = 0;
}
