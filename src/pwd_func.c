#include "ush.h"

void pwd_func(t_main *interface)
{
    long path_max;
    size_t size;
    char *ptr;

    path_max = pathconf(".", _PC_PATH_MAX);
    size = (path_max == -1) ? 1024 : 
           (path_max > 10240) ? 10240 : path_max;

    for (interface->result.value[0] = ptr = NULL; ptr == NULL; size *= 2)
    {
        if ((interface->result.value = realloc(interface->result.value, size)) == NULL)
            strerror(errno);

        ptr = getcwd(interface->result.value[0], size);
 
        if (ptr == NULL && errno != ERANGE)
            strerror(errno);
    }

    interface->status = 1;
}
