#include "ush.h"

struct termios stored_settings;

int main(int argn, char *argv[])
{
    t_main *interface = NULL;
    init_main_struct(&interface);
    // set_keypress();

    while (interface->status) {
        mx_printstr(interface->ush.ush_name);
        read_line(interface);
        execute(interface);
        clean_args_struct(&interface->line_arg);
    }
    
    // reset_keypress();

    return EXIT_SUCCESS;
}
