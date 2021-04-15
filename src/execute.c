#include "ush.h"

void fg_func(t_main *interface)
{
    interface->status = 1;
}

void exit_func(t_main *interface)
{
<<<<<<< HEAD
    interface->status = 0;
=======
    mx_print_strarr(interface->command_args, " ");
    return 0;
>>>>>>> 9cf39806e53290b469830eb1e9b1d914d730aa6e
}

void unset_func(t_main *interface)
{
    interface->status = 1;
}

void export_func(t_main *interface)
{
    interface->status = 1;
}

void env_func(t_main *interface)
{
    interface->status = 1;
}

void which_func(t_main *interface)
{
    interface->status = 1;
}

void echo_func(t_main *interface)
{
    interface->status = 1;
}

void search_command(t_main *interface)
{
    mx_printstr("command: ");
    mx_printstr(interface->func_arg.value[0]);
    mx_printstr(" not found\n");

    interface->status = 1;
}

void (*builtin_func[]) (t_main *) = {
    &exit_func,
    &unset_func,
    &export_func,
    &env_func,
    &cd_func,
    &pwd_func,
    &which_func,
    &echo_func,
    &fg_func,
    &search_command
};

void execute(t_main *interface) // REV 1.02
{
    char *commands[] = {
        "exit",     //0
        "unset",    //1
        "export",   //2
        "env",      //3
        "cd",       //4
        "pwd",      //5
        "whitch",   //6
        "echo",     //7
        "pwd",      //8
        "fg",       //9
        NULL        //10
    };

    for(uint8_t index = 0; interface->line_arg.value[index] && interface->status;) {
        uint8_t i = 0;

        get_func_arg(interface, &index);
        
        for(; commands[i] && mx_strcmp(interface->func_arg.value[0], commands[i]); ++i);
        
        builtin_func[i](interface);

        /*    print result of the last function    */
        if(interface->result) {
            mx_printstr(interface->result);
            mx_printchar('\n');
            mx_strdel(&interface->result);
        }

        free(interface->func_arg.value);
        interface->func_arg.value = NULL;
    }
    
    free(interface->line_arg.value);
}
