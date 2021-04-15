#include "ush.h"

void pwd_func(t_main *interface)
{
    char *ptr;
    int32_t path_max = pathconf(".", _PC_PATH_MAX);
    size_t size = (path_max == -1) ? 1024 : 
           (path_max > 10240) ? 10240 : path_max;

    if ((interface->result.value = (char **) calloc(1, sizeof(char *))) == NULL)
            strerror(errno);  

    for (interface->result.value[0] = ptr = NULL; ptr == NULL; size *= 2)
    {
        if ((interface->result.value[0] = realloc(interface->result.value[0], size)) == NULL)
            strerror(errno);

        ptr = getcwd(interface->result.value[0], size);
 
        if (ptr == NULL && errno != ERANGE)
            strerror(errno);
    }

    interface->result.number = 1;
    interface->status = 1;
}
