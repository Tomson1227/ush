#include "ush.h"

void pwd_func(t_main *interface)
{

    long path_max;
    size_t size;
    char *ptr;

    path_max = pathconf(".", _PC_PATH_MAX);
    if (path_max == -1)
        size = 1024;
    else if (path_max > 10240)
        size = 10240;
    else
        size = path_max;


    for (interface->result = ptr = NULL; ptr == NULL; size *= 2)
    {
        if ((interface->result = realloc(interface->result, size)) == NULL)
            strerror(errno);

        ptr = getcwd(interface->result, size);
        if (ptr == NULL && errno != ERANGE)
            strerror(errno);
    }

    interface->status = 1;
}
