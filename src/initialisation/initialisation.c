#include "ush.h"

static inline void init_args_struct(t_args *args)
{
    args->number = 0;
    args->value = NULL;
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
    init_triggers_struct(&(*interface)->triggers);
    (*interface)->command = NULL;

    size_t prompt_size = snprintf(NULL, 0, "%s%sU$H>%s ", BOLD, GREEN, DEFAULT_COLLOR);
    (*interface)->prompt = (char *) calloc(prompt_size, sizeof(char));
    sprintf((*interface)->prompt, "%s%sU$H>%s ", BOLD, GREEN, DEFAULT_COLLOR);
}

void init_line_struct(t_line *line)
{
    line->line = (char *) calloc(BUFSIZE, sizeof(char));
    line->size = 0;
    line->position = 0;
    line->symbol = '\0';
    line->last_command = NULL;
}

void clear_line_struct(t_line **line)
{
    free((*line)->line);
    (*line)->line = NULL;
    (*line)->size = 0;
    (*line)->position = 0;
    (*line)->symbol = '\0';
    (*line)->last_command = NULL;
    free(*line);
    *line = NULL;
}
