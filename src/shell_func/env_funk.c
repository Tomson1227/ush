#include "ush.h"


static void print_env(void);

/*
env – The command allows you to run another program in a custom environment without 
modifying the current one. When used without an argument it will print a list of the 
current environment variables.

-P  Print out the key and IV used then immediately exit: don't do any encryption or
    decryption.
-i  Ignores the environment that would otherwise be inherited from the current shell. 
    Restricts the environment for utility to that specified by the arguments.
*/
void env_func(t_ush *ush, t_process *process)
{  
    uint32_t argc = count_args(process->args);

    if(argc == 1)
        print_env();

    uint32_t options = get_opt(process->args, "iuP");
    uint8_t opt_i = READ_OPT(options, 1);
    uint8_t opt_u = READ_OPT(options, 2);
    uint8_t opt_P = READ_OPT(options, 4);


    
    ush->local_status = process->status = 0;
}

static void print_env(void)
{
    for (char **env = environ; *env != 0; env++)
        printf("%s\n", *env);
}

// static void process_u(char **env, char *u) {
//     while (env && *env) {
//         char **tmp = mx_split_key_value(*env);

//         if (!mx_strcmp(tmp[0], u)) {
//             free(*env);
//             for (int i = 0; env[i]; i++)
//                 env[i] = env[i + 1];

//             mx_del_strarr(&tmp);
//             break;
//         }

//         mx_del_strarr(&tmp);
//         env++;
//     }
// }

// static void process_P(t_env *env) {
//     char *full_path = NULL;
//     char *tmp = NULL;

//     if (env->P[mx_strlen(env->P) - 1] == '/')
//         full_path = mx_strjoin(env->P, *(env->util));
//     else {
//         tmp = mx_strjoin(env->P, "/");
//         full_path = mx_strjoin(tmp, *(env->util));
//         free(tmp);
//     }

//     free(env->util[0]);
//     env->util[0] = full_path;
// }

// static char **set_environment(t_env *env, t_ush *ush) {
//     char **environment = NULL;

//     if (env->i)
//         environment = mx_clean_duplicates(mx_strarr_dup(env->name_val));
//     else
//         environment = mx_strarr_join(ush->env, env->name_val);

//     return environment;
// }

// static int execute(t_ush *ush, t_env *env) {
//     char *cmd = mx_strarr_to_str(env->util, " ");
//     t_job *job = mx_create_job(cmd);
//     int status = mx_launch_job(job, ush);

//     mx_delete_job(&job);
//     mx_strdel(&cmd);
//     return status;
// }

// int mx_env(t_env *env, t_ush *ush) {
//     char **environment = NULL;
//     char **tmp_env = ush->env;
//     int status = MX_SUCCESS;

//     environment = set_environment(env, ush);
//     ush->env = environment;
//     if (env->u && !env->i)
//         process_u(environment, env->u);
//     if (env->P && env->util)
//         process_P(env);
//     if (!env->util)
//         env->util = mx_strsplit("env", ' ');
//     if (!mx_strcmp("env", env->util[0]) && !env->util[1])
//         mx_print_strarr(environment, "\n");
//     else
//         status = execute(ush, env);

//     ush->env = tmp_env;
//     mx_del_strarr(&environment);
//     return status;
// }
