#ifndef STRUCT_H
#define STRUCT_H


typedef struct s_ush {
    char *ush_name;
}              t_ush;


    /* main struct */
typedef struct s_main {
    t_ush ush;
    char *line;
    char **args;
    volatile int status;
    char **command_args;
}              t_main;

#endif /* All struct */
