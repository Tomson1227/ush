#include "ush.h"

struct termios stored_settings;

int main(int argn, char *argv[])
{
    t_main *interface = NULL;
    init_main_struct(&interface);

    while (interface->status) {
        read_line(interface);
        execute(interface);
        clean_args_struct(&interface->line_arg);
    }

    return EXIT_SUCCESS;
}
