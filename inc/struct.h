#ifndef STRUCT_H
#define STRUCT_H

typedef struct s_main t_main;

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

typedef struct s_func {
    void (*print_args) (t_main *);
    void (*write_arg) (t_args *, char *);
    void (*copy_args) (t_args *, t_args *);
    void (*ncopy_args) (t_args *, t_args *, uint16_t);
}              t_func;

    /* main struct */
struct s_main {
    t_args line_arg;
    t_args func_arg;
    t_args result;
    t_func func;
    t_triggers triggers;
    volatile int status;
    t_ush ush;
};

#endif /* All struct */
