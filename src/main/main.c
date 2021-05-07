#include "ush.h"

struct termios stored_settings;
char **environ;

int main(int argn, char *argv[])
{
    t_main *interface = NULL;
    init_main_struct(&interface);
    // printf("interface created ? = %s\n", interface ? "True" : "False");

    while (interface->status) {
        // mx_printstr("main1\n");
        read_line(interface);
        // mx_printstr("main2\n");
        execute(interface);
        // mx_printstr("main3\n");
        // clean_args_struct(interface->line_arg);
        // mx_printstr("main4\n");
    }

    return EXIT_SUCCESS;
}
