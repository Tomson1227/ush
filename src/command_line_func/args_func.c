#include "ush.h"

void print_args(t_args *args) 
{
    if(args->number == 0)
        return;

    mx_printstr(args->value[0]);   

    for(uint8_t i = 1; args->value[i]; ++i) {
        mx_printchar(' ');
        mx_printstr(args->value[i]);
    }

    del_args_structure(&args);
    // clean_args_struct(args);
}

void calloc_args(t_args *args, uint16_t n)
{
    if(!(args->value = calloc(n, sizeof(char *))))
        strerror(errno);
}

void realloc_args(t_args *args, uint16_t added_pos)
{
    if(!(args->value = realloc(args->value, args->number + added_pos)))
        strerror(errno);
}

void write_arg(t_args *args, char *argv)
{
    args->value[args->number++] = mx_strdup(argv);
    args->value[args->number] = NULL;
}

void copy_args(t_args *args_dest, t_args *args_src)
{
    realloc_args(args_dest, args_src->number + args_src->number);

    for(uint16_t index = 0; args_src->value[index]; ++index) {
        write_arg(args_dest, args_src->value[index]);
    }
}

void ncopy_args(t_args *args_dest, t_args *args_src, uint16_t n)
{
    realloc_args(args_dest, args_dest->number + n);

    for(uint16_t index = 0; index < n; ++index) {
        write_arg(args_dest, args_src->value[index]);
    }
}

void add_newline(char **str)
{
    size_t len = mx_strlen(*str);

    if(!(*str = realloc(*str, len + 1)))
        strerror(errno);

    (*str)[len++] = '\n';
    (*str)[len++] = '\0';
}
