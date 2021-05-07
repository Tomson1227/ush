#include "ush.h"

void init_process(t_process **process)
{
    (*process)->status = 0;
    (*process)->mode = 0;
    (*process)->command = NULL;
    (*process)->file = NULL;
    init_args_struct(&(*process)->result);
    init_args_struct(&(*process)->parameters);
}

void del_process(t_process **process)
{
    (*process)->mode = 0;
    (*process)->status = 0;
    mx_strdel(&(*process)->command);
    // printf("Del Process1\n");
    // del_args_structure(&(*process)->parameters);
    // printf("Del Process2\n");
    // del_args_structure(&(*process)->result);
}

void new_procces_list(t_process_list **process_head)
{
    t_process_list *new_process = NULL;
    t_process_list *temp_process = NULL;

    if(!(new_process = (t_process_list *) calloc(1, sizeof(t_process_list))))
        strerror(errno);

    if(!(new_process->process = (t_process *) calloc(1, sizeof(t_process))))
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
