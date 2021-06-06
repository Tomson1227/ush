#include "ush.h"

/* 
GET:
char **argv - NULL terminated array
char *search_opt - string with optinons chars (max 32)
RETURN:
32-b registr fill with coresponding char option in the same position
*/
uint32_t get_opt(char **argv, char *seach_opt)
{
    extern int optind;
    int c, pos;
    uint32_t find_opt = 0, argc;

    for(argc = 0; argv[argc]; ++argc);

    while((c = getopt(argc, argv, seach_opt)) != -1)
        find_opt |= 1 << mx_get_char_index(seach_opt, c);

    optind = 1;
    return find_opt;
}

uint32_t count_args(char **argv)
{
    uint32_t count = 0;

    if(!argv)
        return count;

    for( ; argv[count]; ++count);

    return count;
}

uint32_t start_index(char **argv)
{
    uint32_t count = 0;

    if(!argv)
        return count;

    for( ; argv[count][0] == '-'; ++count);

    return count;
}

// static char *is_param_exist(char **env, char *param) {
//     if(!env)
//         return NULL;
    
//     for(uint64_t index = 0; env[index]; ++index) {
//         if(!mx_strncmp(env[index], param, strlen(param)))
//             return env[index];
//     }

//     return NULL;
// } 

// static void add_sheel_env(char ***env, char *param, char *value)
// {
//     char *new_param = mx_strnew(strlen(param) + strlen(value) + 2);
//     sprintf(new_param, "%s=%s", param, value);
//     uint32_t count = count_args(*env);

//     if(malloc_size(*env) <= count) {
//         if(!(*env = (char **) realloc(*env, count + 1)))
//             perror("ush :");
//     }

//     (*env)[count++] = new_param;
//     (*env)[count] = NULL;
// }
