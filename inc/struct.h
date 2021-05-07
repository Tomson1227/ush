#ifndef STRUCT_H
#define STRUCT_H

typedef struct s_main t_main;

typedef struct s_args {
    int number;
    char **value;
}              t_args;

typedef struct s_process {
    char *file;
    char *command;
    int status;
    uint64_t mode;
    pid_t pid;
    t_args *result;
    t_args *parameters;
    posix_spawn_file_actions_t actions;
}              t_process;

typedef struct s_process_list {
    t_process *process;
    struct s_process_list *next_process;
    struct s_process_list *prev_process;
}              t_process_list;

typedef struct s_command_list {
    char *command;
    struct s_command_list *next_command;
    struct s_command_list *prev_command;
}              t_command_list;

typedef struct s_tab_func {
    uint16_t var_num;
    uint16_t line_index;
    int16_t var_index;
    uint16_t tab_press;
    char **bin_dirs;
    char **variants;
    char *last_arg;
    char *serch_arg;
    char *path; 
    bool command; // true - search for command // false - search for file name 
}              t_tab_func;

    /* main struct */
struct s_main {
    t_args *line_arg;
    t_args *func_arg;
    t_args *result;
    volatile int status;
    char *prompt;
    t_command_list *command;
    t_process_list *process_list;
};

typedef struct s_line {
    t_command_list *last_command;
    char *line;
    char *key_press;
    size_t size;
    size_t position;
    int symbol;
    t_tab_func *tab_func;
    struct termios term;
    struct termios oterm1;
    struct termios oterm2;
}              t_line;

#endif /* All struct */
