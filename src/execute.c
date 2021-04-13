#include "ush.h"

int fs_function(char **command_args)
{
    (void)command_args;

    return 1;
}

int execute(char **args)
{
    int status = 1;
    char **command_args = NULL;

    for(uint8_t index = 0; args[index]; ++index) {
        command_args = get_args(args, &index);

        if(!command_args)
            break;
        else if(!mx_strcmp(command_args[0], "exit"))
            status = 0;
        else if(!mx_strcmp(command_args[0], "fg")) {
            status = fs_function(command_args); //need to be developed
        }
        else if(!mx_strcmp(command_args[0], "unset"));
        else if(!mx_strcmp(command_args[0], "export"));
        else if(!mx_strcmp(command_args[0], "env"));
        else if(!mx_strcmp(command_args[0], "cd"));
        else if(!mx_strcmp(command_args[0], "pwd"));
        else if(!mx_strcmp(command_args[0], "whitch"));
        else if(!mx_strcmp(command_args[0], "echo"));
        else if(!mx_strcmp(command_args[0], "pwd"));
        else;
    }
    
    return status;
}
