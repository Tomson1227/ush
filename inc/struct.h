#ifndef STRUCT_H
#define STRUCT_H


typedef struct s_ush {
    char *ush_name;
}              t_ush;

typedef struct s_args {
    int number;
    char **value;
}              t_args;

typedef struct s_triggers {
    bool pipe; // '|'
}              t_triggers;     

    /* main struct */
typedef struct s_main {
    t_args func_arg;
    t_args line_arg;
    t_args result;
    t_triggers triggers;
    volatile int status;
    t_ush ush;
}              t_main;

#endif /* All struct */
