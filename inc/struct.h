#ifndef STRUCT_H
#define STRUCT_H

typedef struct s_main t_main;

typedef struct s_args {
    int number;
    char **value;
}              t_args;

typedef struct s_triggers {
    bool pipe; // '|'
}              t_triggers;     

typedef struct s_command_list {
    char *command;
    struct s_command_list *next_command;
    struct s_command_list *prev_command;
}              t_command_list;

    /* main struct */
struct s_main {
    t_args line_arg;
    t_args func_arg;
    t_args result;
    t_triggers triggers;
    volatile int status;
    char *prompt;
    t_command_list *command;
};

typedef struct s_line {
    char *prompt;
    char *line;
    size_t size;
    size_t position;
    int symbol;
    struct termios term;
    struct termios oterm;
}              t_line;

#endif /* All struct */
