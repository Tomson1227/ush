#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void start_gui(void);
char *read_line(void);
int execute(char **args);
char **get_args(char **args, uint8_t *index);

#endif /* FUNCTION_H */
