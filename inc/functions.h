#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "struct.h"

void start_gui(void);
char *read_line(void);
void execute(t_main *interface);
void get_func_arg(t_main *interface, uint8_t *index);
t_main *new_struct_t_main(void);

void pwd_func(t_main *interface);
void cd_func(t_main *interface);
void echo_func(t_main *interface);

#endif /* FUNCTION_H */
