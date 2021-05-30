#include "ush.h"

static void read_pipe_line(t_ush *ush)
{
    char *line = NULL;
    mx_read_line(&line, 1, '\0', STDIN_FILENO);
    char *clean_line = mx_del_extra_spaces(line);
    mx_strdel(&line);
    ush->args = mx_strsplit(clean_line, ' ');  
    mx_strdel(&clean_line);
}

void pipe_call(t_ush *ush)
{
    read_pipe_line(ush);
    execute(ush);
}
