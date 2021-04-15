#ifndef STRUCT_H
#define STRUCT_H


typedef struct s_ush {
    char *ush_name;
}              t_ush;

typedef struct s_args {
    int number;
    char **value;
}              t_args;

// enum e_instruct {
//     pipe = (uint8_t) 0x0001U
// };


// typedef struct s_instruction {
//     uint8_t op_flag;
// }              t_instruction;


    /* main struct */
typedef struct s_main {
    t_args func_arg;
    t_args line_arg;
    t_args result;
    volatile int status;
    t_ush ush;
}              t_main;

#endif /* All struct */
