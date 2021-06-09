#include "ush.h"

void pipe_call(t_ush *ush)
{
    char *line = NULL;
    mx_read_line(&line, 1, '\0', STDIN_FILENO);
    validate_args(ush, line);
    // print_arg(ush->args);
    execute(ush);
    ush->status = ush->local_status;
}
