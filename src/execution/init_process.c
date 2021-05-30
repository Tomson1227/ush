#include "ush.h"

static void init_process(t_process **process)
{
    if(!((*process)->actions = (posix_spawn_file_actions_t *) calloc(1, sizeof(posix_spawn_file_actions_t))) ||
       !((*process)->attr = (posix_spawnattr_t *) calloc(1, sizeof(posix_spawnattr_t))) ||
       posix_spawn_file_actions_init((*process)->actions) ||
       posix_spawnattr_init((*process)->attr))
        strerror(errno), exit(1);

    (*process)->status = 0;
    (*process)->command = NULL;
    (*process)->file = NULL;
    (*process)->args = NULL;
}

void new_procces_list(t_process_list **process_head)
{
    t_process_list *new_process = NULL;
    t_process_list *temp_process = NULL;

    if(!(new_process = (t_process_list *) calloc(1, sizeof(t_process_list))) ||
       !(new_process->process = (t_process *) calloc(1, sizeof(t_process))))
        strerror(errno);

    init_process(&new_process->process);

    if(!(*process_head)) {
        (*process_head) = new_process;        
        return;
    }

    temp_process = (*process_head);
    
    new_process->prev_process = *process_head;
    (*process_head)->next_process = new_process;
    (*process_head) = new_process;
}

static void del_process(t_process **process)
{
    (*process)->status = 0;
    (*process)->command = NULL;
    mx_del_strarr(&(*process)->args);
    posix_spawn_file_actions_destroy((*process)->actions);
    posix_spawnattr_destroy((*process)->attr);
}

void del_process_list(t_process_list **process_head) 
{
    for(; *process_head; ) {
        del_process(&(*process_head)->process);
        (*process_head)->prev_process = NULL;
        (*process_head) = (*process_head)->next_process;
    }

    free(*process_head);
    *process_head = NULL;
}
