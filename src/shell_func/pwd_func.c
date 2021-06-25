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
  
    char *pwd = get_env_value(ush, "PWD");

    if(opt_P) {
        char *link = realpath(pwd, NULL);
        printf("%s\n", link);
        free(link);
    }
    else
        printf("%s\n", pwd);

    process->status = 0;
}
