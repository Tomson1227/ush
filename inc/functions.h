#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "struct.h"

void start_gui(void);
char *read_line(void);
int execute(char **args);
char **get_args(char **args, uint8_t *index);
t_main *new_struct_t_main(void);

#endif /* FUNCTION_H */
