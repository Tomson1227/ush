#include "ush.h"

void search_command(t_main *interface)
{
    mx_printstr("command: ");
    mx_printstr(interface->func_arg->value[0]);
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

// void execute(t_main *interface) // REV 1.02
// {
//     char *commands[] = {
//         "exit",     //0
//         "unset",    //1
//         "export",   //2
//         "env",      //3
//         "cd",       //4
//         "pwd",      //5
//         "whitch",   //6
//         "echo",     //7
//         "fg",       //8
//         NULL        //9
//     };

//     for(uint8_t index = 0; interface->line_arg->value[index] && interface->status;) {
//         uint8_t i = 0;
//         get_func_arg(interface, &index);

//         for(; commands[i] && mx_strcmp(interface->func_arg->value[0], commands[i]); ++i);
        
//         builtin_func[i](interface);
//         clean_args_struct(interface->func_arg);
        
//         /*    print result of the last function    */
//         print_args(interface);
//     }
// }



static void create_process_list(t_main *interface)
{
    if(!interface->line_arg->value) {
        interface->status = 1;
        return;
    }

    for(uint8_t index = 0; interface->line_arg->value[index]; ) {
        new_process(interface, &index);
        get_func_arg(interface, &index);
    }
}

/* Debug func */
void print_process_list(t_main *interface)
{
    if(!interface->process_list) {
        mx_printstr("Empty list");
    }

    t_process_list *curent_list = interface->process_list;
    
    while(curent_list->prev_process)
        curent_list = curent_list->prev_process;


    for(uint8_t process_n = 1; curent_list; curent_list = curent_list->next_process, ++process_n) {
        printf("PROCESS %d:\nCMD = %s\nPARAM:\n", process_n, curent_list->process->command);
            
        if(curent_list->process->parameters->value[0]) {
            mx_printstr(curent_list->process->parameters->value[0]);   

            for(uint8_t i = 1; curent_list->process->parameters->value[i]; ++i) {
                mx_printchar(' ');
                mx_printstr(curent_list->process->parameters->value[i]);
            }
        }
        
        if(curent_list->process->file) {
            mx_printstr("\nFILE: ");
            mx_printstr(curent_list->process->file);
            mx_printchar('\n');
        }

        mx_printstr("\nMODE: ");

        for(uint8_t i = 0; i < 8; ++i) {
            mx_printchar(curent_list->process->mode & (0x80 >> i) ? '1' : '0');
        }
        mx_printchar('\n');
    }

    mx_printchar('\n');
}

static void run_command(t_process *process)
{
    // pid_t pid;
    int status;

    printf("Run command: %s\n", process->command);
    status = posix_spawn(&process->pid, process->command, NULL, NULL, process->parameters->value, environ);

    if(status == 0) {
        printf("Child pid: %i\n", process->pid);

        if(waitpid(process->pid, &status, 0) != -1) {
            printf("Child exited with status %i\n", status);
        } 
        else {
            perror("waitpid");
        }
    }
    else {
        printf("posix_spawn: %s\n", strerror(status));
    }
}


int main(int argc, char* argv[], char *env[])
{
    int ret;
    pid_t child_pid;
    posix_spawn_file_actions_t child_fd_actions;
    if (ret = posix_spawn_file_actions_init (&child_fd_actions))
        perror ("posix_spawn_file_actions_init"), exit(ret);
    if (ret = posix_spawn_file_actions_addopen (&child_fd_actions, 1, "/tmp/foo-log", 
            O_WRONLY | O_CREAT | O_TRUNC, 0644))
        perror ("posix_spawn_file_actions_addopen"), exit(ret);
    if (ret = posix_spawn_file_actions_adddup2 (&child_fd_actions, 1, 2))
        perror ("posix_spawn_file_actions_adddup2"), exit(ret);

    if (ret = posix_spawnp (&child_pid, "date", &child_fd_actions, NULL, argv, env))
        perror ("posix_spawn"), exit(ret);
}

static void start_process(t_process *process)
{
    process->status = posix_spawn(&process->pid, process->command, &process->actions, NULL, process->parameters->value, environ);
}

static void run_process_list(t_process_list *process_list)
{
    while(process_list->next_process) {
        start_process(process_list->process);
        process_list = process_list->next_process;
    }
}

void execute(t_main *interface) // REV 2.0
{
    create_process_list(interface);
    print_process_list(interface);
    run_process_list(interface->process_list);
    // wait_process_end();
    // check_status();
    // print_result();
    // clear_process_list();
    del_process_list(&interface->process_list);
}

