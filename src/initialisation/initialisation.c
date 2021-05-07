#include "ush.h"

void init_args_struct(t_args **args)
{
    if(!(*args = (t_args *) calloc(1, sizeof(t_args))))
        strerror(errno);
    
    (*args)->number = 0;
    (*args)->value = NULL;
}

void del_args_structure(t_args **args)
{
    (*args)->number = 0;
    
    for(uint16_t index = 0; (*args)->value[index]; ++index) {
        mx_strdel(&(*args)->value[index]);
    }

    free((*args)->value);
    (*args)->value = NULL;
    
    // mx_del_strarr(&(*args)->value);
    free(*args);
    *args = NULL;
}

void set_prompt(t_main *interface)
{
    char *read_pwd = get_pwd();
    size_t prompt_size = snprintf(NULL, 0, "%s%sU$H>%s:%s%s%s$ ", 
                                BOLD, FG_COLOR_GREEN, FG_COLOR_RESET,
                                FG_COLOR_BLUE, read_pwd, RESET_ALL);
    interface->prompt = (char *) calloc(prompt_size, sizeof(char));
    sprintf(interface->prompt, "%s%sU$H>%s:%s%s%s$ ",
            BOLD, FG_COLOR_GREEN, FG_COLOR_RESET,
            FG_COLOR_BLUE, read_pwd, RESET_ALL);
}

void init_main_struct(t_main **interface)
{  
    if(!(*interface = (t_main *) calloc(1, sizeof(t_main))))  
        strerror(errno);

    (*interface)->status = 1;
    init_args_struct(&(*interface)->func_arg);
    init_args_struct(&(*interface)->line_arg);
    init_args_struct(&(*interface)->result);
    (*interface)->command = NULL;
    set_prompt(*interface);
    (*interface)->process_list = NULL;
}

static inline t_tab_func *init_tab_struct(void)
{
    t_tab_func *tab_func = NULL;
    
    if(!(tab_func = (t_tab_func *) calloc(1, sizeof(t_tab_func))))
        strerror(errno);
    tab_func->var_index = 0;
    tab_func->var_num = 0;
    tab_func->variants = NULL;
    tab_func->last_arg = NULL;
    tab_func->serch_arg = NULL;
    tab_func->path = NULL;
    tab_func->bin_dirs = mx_strsplit(getenv("PATH"), ':');
    tab_func->command = false;

    return tab_func;
}

void init_line_struct(t_line *line, t_main *interface)
{
    tcgetattr(0, &line->term);
    memcpy(&line->oterm1, &line->term, sizeof(struct termios));
    line->oterm1.c_lflag &= ~(ICANON | ECHO); //ISIG
    line->oterm1.c_cc[VMIN] = 1;
    line->oterm1.c_cc[VTIME] = 0;

    line->line = (char *) calloc(BUFSIZE, sizeof(char));
    line->size = 0;
    line->position = 0;
    line->symbol = '\0';
    line->last_command = interface->command;
    line->key_press = (char *) calloc(10, sizeof(char));
    line->tab_func = init_tab_struct();
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
