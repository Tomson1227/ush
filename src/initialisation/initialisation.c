#include "ush.h"

static inline void init_args_struct(t_args *args)
{
    args->number = 0;
    args->value = NULL;
}

static inline void init_func_struct(t_func *func)
{
    func->print_args = print_args;
    func->write_arg = write_arg;
    func->copy_args = copy_args;
    func->ncopy_args = ncopy_args;
}

static inline void init_triggers_struct(t_triggers *triggers)
{
    triggers->pipe = false;
}

void init_main_struct(t_main **interface)
{  
    if(!(*interface = (t_main *) calloc(1, sizeof(t_main))))  
        strerror(errno);

    (*interface)->status = 1;
    init_args_struct(&(*interface)->func_arg);
    init_args_struct(&(*interface)->line_arg);
    init_args_struct(&(*interface)->result);
    init_func_struct(&(*interface)->func);
    init_triggers_struct(&(*interface)->triggers);
    
    size_t prompt_size = snprintf(NULL, 0, "%s%su$h>%s", BOLD, GREEN, DEFAULT_COLLOR);
    (*interface)->ush.ush_name = (char *) calloc(prompt_size, sizeof(char));
    sprintf((*interface)->ush.ush_name, "%s%su$h>%s", BOLD, GREEN, DEFAULT_COLLOR);
    // (*interface)->ush.ush_name = mx_strdup("\033[1m\033[32mu$h>\033[0m ");
}

