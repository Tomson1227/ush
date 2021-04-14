#ifndef STRUCT_H
#define STRUCT_H


typedef struct s_ush {
    char *ush_name;
}              t_ush;

typedef struct s_args {
    int number;
    char **value;
}              t_args;

    /* main struct */
typedef struct s_main {
    t_args func_arg;
    t_args line_arg;
    t_ush ush;
    volatile int status;
    char *result;
}              t_main;

#endif /* All struct */
