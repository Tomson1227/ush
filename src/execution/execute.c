#include "ush.h"

static void create_process_list(t_ush *ush);
static void run_process_list(t_ush *ush);
static void print_process_list(t_ush *ush);

/* REV 2.0 */
void execute(t_ush *ush) 
{
    if(!ush->args || !(ush->args[0]))
        return;

    create_process_list(ush);
    // print_process_list(ush);
    run_process_list(ush);
    del_process_list(&ush->process_list);
}

static void (*built_in_func[]) (t_ush *, t_process *) = {
    &exit_func,
    &unset_func,
    &export_func,
    &env_func,
    &cd_func,
    &pwd_func,
    &which_func,
    &echo_func,
    &fg_func,
    &set_env
};

static void create_process_list(t_ush *ush)
{
    if(!ush->args)
        return;

    char **args_head = ush->args;
    
    for(; *ush->args;)
        create_process(ush);

    free(args_head);
}

static void start_process(t_process *process)
{
    process->status = posix_spawnp(&process->pid, process->command, 
                                   process->actions, process->attr, 
                                   process->args, environ);

    wait(&process->pid);   

    if(process->status == 2)
        printf("ush: %s: command not found\n", process->command);        

}

/* Return index of build in command otherwise -1 */
int built_in_func_index(t_ush *ush, char *command)
{
    for(uint8_t index = 0; ush->built_in[index]; ++index) {
        if(!mx_strcmp(ush->built_in[index], command))
            return index;

        if(mx_get_char_index(command, '=') != -1)
            return 9;
    }

    return -1;
}

static void run_process_list(t_ush *ush)
{
    while(ush->process_list && ush->process_list->prev_process)
            ush->process_list = ush->process_list->prev_process;

    while(ush->process_list) {
        int command_index = built_in_func_index(ush, ush->process_list->process->command);

        replace_args_escapes(ush, ush->process_list->process);

        if(command_index != -1)
            built_in_func[command_index](ush, ush->process_list->process);
        else
            start_process(ush->process_list->process);

        ush->local_status = ush->process_list->process->status;
        ush->process_list = ush->process_list->next_process;
    }
}

/* Debug func */
static void print_process_list(t_ush *ush)
{
    if(!ush->process_list)
        mx_printstr("Empty list");

    t_process_list *curent_list = ush->process_list;
    
    while(curent_list->prev_process)
        curent_list = curent_list->prev_process;

    for(uint8_t process_n = 1; curent_list; curent_list = curent_list->next_process, ++process_n) {
        printf("PROCESS %d:\nCMD = %s\nPARAM:\n", process_n, curent_list->process->command);
            
        if(curent_list->process->args[0]) {
            mx_printstr(curent_list->process->args[0]);   

            for(uint8_t i = 1; curent_list->process->args[i]; ++i) {
                mx_printchar(' ');
                mx_printstr(curent_list->process->args[i]);
            }
        }
        
        if(curent_list->process->file) {
            mx_printstr("\nFILE: ");
            mx_printstr(curent_list->process->file);
            mx_printchar('\n');
        }

    }

    mx_printchar('\n');
}
