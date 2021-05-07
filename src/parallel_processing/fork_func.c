#include "ush.h"

int create_fork(int *child_id)
{
    int id = fork();

    if(id == -1)
        strerror(errno);

    *child_id = getpid();

    return id;
}

void wait_proc_to_die(int id)
{
    int dead_id;

    if(wait(&dead_id) == -1)
        strerror(errno);
    
    while(dead_id != id) {
        if(wait(&dead_id) == -1)
            strerror(errno);
    }
}
