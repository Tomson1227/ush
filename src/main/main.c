#include "ush.h"

struct termios stored_settings;

int main(int argn, char *argv[])
{
    t_main *interface = NULL;
    init_main_struct(&interface);

    while (interface->status) {
        printf("TP1\n");
        read_line(interface);
        printf("TP2\n");
        execute(interface);
        clean_args_struct(&interface->line_arg);
    }

    return EXIT_SUCCESS;
}
