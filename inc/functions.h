#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "struct.h"

void print_args(t_args *args);
void start_gui(void);
void gui_call(t_ush *ush);
void pipe_call(t_ush *ush);
void execute(t_ush *ush);
void validate_args(t_ush *ush, t_line *line);
void get_user_input(t_line *line, t_ush *ush);

void exit_func(t_ush *ush, t_process *process);
void pwd_func(t_ush *ush, t_process *process);
void cd_func(t_ush *ush, t_process *process);
void echo_func(t_ush *ush, t_process *process);
void which_func(t_ush *ush, t_process *process);
void env_func(t_ush *ush, t_process *process);
void export_func(t_ush *ush, t_process *process);
void unset_func(t_ush *ush, t_process *process);
void fg_func(t_ush *ush, t_process *process);

void init_ush_struct(t_ush **ush);
void del_ush_struct(t_ush **ush);
void set_prompt(t_ush *ush);

void init_line_struct(t_line *line, t_ush *ush);
void del_line_struct(t_line **line);
void reset_line(t_line *line);
void reset_cursore_position(t_line *line);

void new_command_list(t_command_list **head);
void del_last_command(t_command_list **head);
void del_command_list(t_command_list **head);
void set_command(t_command_list *head, char *command);
void dup_command(t_command_list *head, char *command);
void autocomplete(t_line *line);

void new_process(t_ush *ush, uint8_t *index);
void create_process(t_ush *ush);
void new_procces_list(t_process_list **process_head);
void del_process_list(t_process_list **process_head);
void get_process_args(t_ush *ush, t_process *process);

char *get_env_value(char *env_key);
char *get_pwd(void); //memory allocation func
char *find_value(char *str);
void replace_str(char *src, char *start_point, size_t length, char *str);

#endif /* FUNCTION_H */
