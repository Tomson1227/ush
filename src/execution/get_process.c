#include "ush.h"

void get_process_args(t_ush *ush, t_process *process)
{
    uint16_t index = 1;

    for(; *ush->args; ) {
        if(!mx_strcmp(*ush->args, ";")) {
            mx_strdel(ush->args++);
            break;
        }
        else if(!mx_strcmp(*ush->args, "|")) {
            mx_strdel(ush->args++);
            break;
        }
        else if(!mx_strcmp(*ush->args, ">")) { /* coppy file path */
            mx_strdel(ush->args++);
            posix_spawn_file_actions_addopen(process->actions, 1, *ush->args, 
                O_WRONLY | O_CREAT | O_TRUNC, 0644);
        } 
        else if(!mx_strcmp(*ush->args, "<")) { /* add parametr from file */
            mx_strdel(ush->args++);
            mx_read_line(&process->args[index++], 1, '\0', open(*ush->args, O_RDONLY));
            process->args[index] = NULL;
            mx_strdel(ush->args++);
        }
        else if(!mx_strcmp(*ush->args, ">>")) { /* coppy file path */
            mx_strdel(ush->args++);
            posix_spawn_file_actions_addopen(process->actions, 1, *ush->args, 
                O_WRONLY | O_CREAT | O_APPEND , 0644);
        }
        else if(!mx_strcmp(*ush->args, "<<")) { /* add string to param */
            mx_strdel(ush->args++);
            process->args[index++] = *ush->args++;
            process->args[index] = NULL;
        }
        else if(!mx_strcmp(*ush->args, "&&")) { /* sceep next process if status of previous is false */
            mx_strdel(ush->args++);
            break;
        }
        else if(!mx_strcmp(*ush->args, "||")) { /* sceep next process if status of previous is true */
            mx_strdel(ush->args++);
            break;
        }
        else {
            process->args[index++] = *ush->args++;
            process->args[index] = NULL; 
        }
    }
}

static size_t count_process_args(char **args)
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

static void start_process(t_process *process, char **args)
{    
    if(!args || !(*args))
        return;

    if(!(process->args = (char **) calloc(count_process_args(args) + 1, sizeof(char *))))
        strerror(errno);

    process->command = *args;
    process->args[0] = *args;
    process->args[1] = NULL;
}

static void check_command(t_ush *ush)
{
    for(; *ush->args; ush->args++) {
        if(built_in_func_index(ush, *ush->args) != -1)
            return;

        for(uint8_t i = 0; ush->bin_dirs[i]; ++i) {
            char *temp = mx_strjoin(ush->bin_dirs[i], "/");
            char *path = mx_strjoin(temp, *ush->args);
            mx_strdel(&temp);

            if(!access(path, F_OK)) {
                mx_strdel(&path);
                return;
            }

            mx_strdel(&path);
        }
                
        int index;

        if((index = mx_get_char_index(*ush->args, '=')) >= 0) {
            char *variable = mx_strndup(*ush->args, index);
            char *value = mx_strdup(&ush->args[0][index + 1]);
            set_shell_variable(ush, variable, value);
            mx_strdel(&variable);
            mx_strdel(&value);
            mx_strdel(ush->args);
        }
    }
}

void create_process(t_ush *ush)
{
    new_procces_list(&ush->process_list);
    start_process(ush->process_list->process, ush->args);
    ush->args++;
    get_process_args(ush, ush->process_list->process);
}
