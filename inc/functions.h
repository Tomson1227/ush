#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "struct.h"

void print_args(t_main *interface);
void start_gui(void);
void read_line(t_main *interface);
void execute(t_main *interface);
void get_func_arg(t_main *interface, uint8_t *index);
void add_newline(char **str);

void pwd_func(t_main *interface);
void cd_func(t_main *interface);
void echo_func(t_main *interface);

void calloc_args(t_args *args, uint16_t n);
void realloc_args(t_args *args, uint16_t added_pos);
void write_arg(t_args *args, char *argv);
void init_main_struct(t_main **interface);
void clean_args_struct(t_args *args);
void copy_args(t_args *args_dest, t_args *args_src);
void ncopy_args(t_args *args_dest, t_args *args_src, uint16_t n);

void init_line_struct(t_line *line, t_main *interface);
void clear_line_struct(t_line **line);
void new_command(t_command_list **head);
void del_last_command(t_command_list **head);
void del_command_list(t_command_list **head);
void set_command(t_command_list *head, char *command);
void dup_command(t_command_list *head, char *command);

void set_keypress(void);
void reset_keypress(void);

#endif /* FUNCTION_H */
