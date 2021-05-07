#include "ush.h"

// void get_func_arg(t_main *interface, uint8_t *index)
// {
//     uint8_t count_args;     

//     for(count_args = 0; interface->line_arg->value[(*index) + count_args]; ++count_args) {
//         if(!mx_strcmp(interface->line_arg->value[(*index) + count_args], ";"))
//             break;
//     }

//     calloc_args(interface->func_arg, count_args + 1);

//     for(uint8_t i = 0; i < count_args; ++i) {
//         write_arg(interface->func_arg, interface->line_arg->value[(*index)++]);
//     }
// }

static uint16_t count_process_args(char **args)
{
    uint16_t count = 0;

    for(uint16_t index = 0; args[index]; ++index) {
        if(!mx_strcmp(args[index], ";"))
            break;
        else if(!mx_strcmp(args[index], "|") ||
                !mx_strcmp(args[index], ">") ||
                !mx_strcmp(args[index], "<") ||
                !mx_strcmp(args[index], ">>") ||
                !mx_strcmp(args[index], "<<") ||
                !mx_strcmp(args[index], "||") ||
                !mx_strcmp(args[index], "&&"))
            continue;
        else    
            ++count;
    }

    return count;
}

void new_process(t_main *interface, uint8_t *index)
{
    new_procces_list(&interface->process_list);
    interface->process_list->process->command = interface->line_arg->value[(*index)++];
    calloc_args(interface->process_list->process->parameters, count_process_args(&interface->line_arg->value[(*index)]) + 1);
    interface->process_list->process->parameters->value[0] = NULL;
}

static void get_file_parameters(t_args *args, char *file_name)
{
    // char *file_content = mx_file_to_str(file_name);
    char *file_content = NULL;
    mx_read_line(&file_content, 1, '\0', open(file_name, O_RDONLY));
    args->value[args->number++] = mx_strtrim(file_content);
    // mx_replace_substr(file_content, "\n", " ");
    // uint32_t param_count = mx_count_words(file_content, ' ');
    // char **file_parameters = mx_strsplit(mx_del_extra_spaces(file_content), ' ');
    // mx_strdel(&file_content);
    
    // for(uint32_t index = 0; index < param_count; ++index) {
    //     args->value[args->number++] = file_parameters[index];
    // }
}

void get_func_arg(t_main *interface, uint8_t *index)
{
    uint8_t offset = *index;

    for(; interface->line_arg->value[offset]; ++offset) {
        if(!mx_strcmp(interface->line_arg->value[offset], ";")) {
            ++offset;
            break;
        }
        else if(!mx_strcmp(interface->line_arg->value[offset], "|")) {
            interface->process_list->process->mode |= PRINT_PIPE;
            mx_strdel(&interface->line_arg->value[offset++]);
            new_process(interface, &offset);
            interface->process_list->process->mode |= PIPE_READ;
            get_func_arg(interface, &offset);
            break;
        }
        else if(!mx_strcmp(interface->line_arg->value[offset], ">")) {
            interface->process_list->process->mode |= OVERWRITE_FILE;
            mx_strdel(&interface->line_arg->value[offset++]);
            interface->process_list->process->file = interface->line_arg->value[offset];
            /* coppy file path */
        }
        else if(!mx_strcmp(interface->line_arg->value[offset], "<")) {
            mx_strdel(&interface->line_arg->value[offset++]);
            get_file_parameters(interface->process_list->process->parameters, interface->line_arg->value[offset]);
            /* add parametr from file */
        }
        else if(!mx_strcmp(interface->line_arg->value[offset], ">>")) {
            interface->process_list->process->mode |= WRITE_FILE;
            mx_strdel(&interface->line_arg->value[offset++]);
            interface->process_list->process->file = interface->line_arg->value[offset];
            /* coppy file path */
        }
        else if(!mx_strcmp(interface->line_arg->value[offset], "<<")) {
            mx_strdel(&interface->line_arg->value[offset++]);
            interface->process_list->process->parameters->value[interface->process_list->process->parameters->number++] 
            = interface->line_arg->value[offset];
            interface->process_list->process->parameters->value[interface->process_list->process->parameters->number] = NULL;
            /* add string to param */
        }
        else if(!mx_strcmp(interface->line_arg->value[offset], "&&")) {
            mx_strdel(&interface->line_arg->value[offset++]);
            new_process(interface, &offset);
            interface->process_list->process->mode |= LOGIC_OR;   /* begin next process if status of previous is true */
            get_func_arg(interface, &offset);
            break;
        }
        else if(!mx_strcmp(interface->line_arg->value[offset], "||")) {
            mx_strdel(&interface->line_arg->value[offset++]);
            new_process(interface, &offset);
            interface->process_list->process->mode |= LOGIC_AND;  /* sceep next process if status of previous is true */
            get_func_arg(interface, &offset);
            break;
        }
        else {
            interface->process_list->process->parameters->value[interface->process_list->process->parameters->number++] 
            = interface->line_arg->value[offset];
            interface->process_list->process->parameters->value[interface->process_list->process->parameters->number] = NULL; 
        }
    }

    *index = offset;
}
