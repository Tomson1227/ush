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
    
    for(uint32_t index = 0; (*args)->value[index]; ++index) {
        mx_strdel(&(*args)->value[index]);
    }

    free((*args)->value);
    (*args)->value = NULL;
    
    free(*args);
    *args = NULL;
}

void set_prompt(t_ush *ush)
{
    char *read_pwd = get_pwd();
    int index = mx_get_substr_index(read_pwd, ush->home);

    if(index >= 0)
        replace_str(&read_pwd, index, strlen(ush->home), "~");

    size_t prompt_size = snprintf(NULL, 0, "%s%sU$H>%s:%s%s%s$ ", 
                                BOLD, FG_COLOR_GREEN, FG_COLOR_RESET,
                                FG_COLOR_BLUE, read_pwd, RESET_ALL);
    ush->prompt = (char *) calloc(prompt_size, sizeof(char));
    
    sprintf(ush->prompt, "%s%sU$H>%s:%s%s%s$ ",
            BOLD, FG_COLOR_GREEN, FG_COLOR_RESET,
            FG_COLOR_BLUE, read_pwd, RESET_ALL);

    mx_strdel(&read_pwd);
}

static void init_term_modes(t_ush *ush)
{    
    tcgetattr(0, &ush->term);
    memcpy(&ush->oterm, &ush->term, sizeof(struct termios));
    ush->oterm.c_lflag &= ~(ICANON | ECHO); //ISIG
    ush->oterm.c_cc[VMIN] = 1;
    ush->oterm.c_cc[VTIME] = 0;
}

static inline void init_built_in(char ***built_in)
{
    char *build_in_func[] = {
        "exit", "unset", "export", "env", "cd", "pwd", "which", "echo", "fg", NULL 
    };

    (*built_in) = (char **) calloc(10, sizeof(char *));
    
    for(uint8_t index = 0; index < 9; ++index) {
        (*built_in)[index] = mx_strdup(build_in_func[index]);
    }

    (*built_in)[9] = NULL;
}

void init_ush_struct(t_ush **ush)
{ 
    if(!(*ush = (t_ush *) calloc(1, sizeof(t_ush))))  
        strerror(errno);

    (*ush)->home = get_env_value("HOME");
    (*ush)->bin_dirs = mx_strsplit(getenv("PATH"), ':');
    init_built_in(&(*ush)->built_in);
    (*ush)->status = -1;
    (*ush)->local_status = 0;
    set_prompt(*ush);
    init_term_modes((*ush));
    (*ush)->args = NULL;
    (*ush)->command_list = NULL;
    (*ush)->process_list = NULL;
}

void del_ush_struct(t_ush **ush)
{
    (*ush)->args = NULL;
    del_command_list(&(*ush)->command_list);
    mx_strdel(&(*ush)->prompt);
    mx_strdel(&(*ush)->home);
    mx_del_strarr(&(*ush)->built_in);
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

void init_line_struct(t_line *line, t_ush *ush)
{
    line->line = (char *) calloc(BUFSIZE, sizeof(char));
    line->get_char = 0;
    line->position = 0;
    line->last_command = ush->command_list;
    line->tab_func = init_tab_struct();
}

void del_line_struct(t_line **line)
{
    free((*line)->line);
    (*line)->line = NULL;
    (*line)->position = 0;
    (*line)->last_command = NULL;
    free(*line);
    *line = NULL;
}
