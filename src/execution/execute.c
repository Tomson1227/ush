#include "ush.h"

/* Debug func */
void print_process_list(t_ush *ush)
{
    if(!ush->process_list) {
        mx_printstr("Empty list");
    }

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

void (*builtin_func[]) (t_ush *, t_process *) = {
    &exit_func,
    &unset_func,
    &export_func,
    // &env_func,
    &cd_func,
    // &pwd_func,
    &which_func,
    // &echo_func,
    &fg_func
};

static void create_process_list(t_ush *ush)
{
    if(!ush->args) {
        return;
    }

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

    if(process->status == 2) {
        mx_printstr("ush: ");
        mx_printstr(process->command);
        mx_printstr(": command not found\n");
    }
}

/* Return index of build in command othrwise -1 */
static int build_in_func_index(char *command)
{
    char *build_in_func[] = {
        "exit",     //0
        "unset",    //1
        "export",   //2
        // "env",      //3
        "cd",       //4
        // "pwd",      //5
        "whitch",   //6
        // "echo",     //7
        "fg",       //8
        NULL 
    };

    for(uint8_t index = 0; build_in_func[index]; ++index) {
        if(!mx_strcmp(build_in_func[index], command))
            return index;
    }

    return -1;
}

static void run_process_list(t_ush *ush)
{
    while(ush->process_list->prev_process)
            ush->process_list = ush->process_list->prev_process;

    while(ush->process_list) {
        int command_index;

        if((command_index = build_in_func_index(ush->process_list->process->command)) != -1) {
            builtin_func[command_index](ush, ush->process_list->process);
        }
        else
            start_process(ush->process_list->process);

        ush->process_list = ush->process_list->next_process;
    }
}

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
