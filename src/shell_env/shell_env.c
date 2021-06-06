#include "ush.h"

/*
shell enviroment variables:

SHELL - shell location
TEMP - terminl type
USER - current user
LOGNAME - same as USER
PWD - current working directory
OLDPWD - previous working directory
HOME - user home directory
PATH - list of directories where the shell will be looking for commands in sequential priority
_ - last executed command
*/

static t_shell_env_list *create_shell_env_list(void) 
{
    t_shell_env_list *new_list = NULL;
    
    if(!(new_list = (t_shell_env_list *) calloc(1, sizeof(t_shell_env_list))))
        perror("ush: ");

    new_list->variable = NULL;
    new_list->value = NULL;
    new_list->exp = 0;
    new_list->next_list = NULL;

    return new_list;
}

static t_shell_env_list *new_shell_env_list(t_ush *ush)
{
    t_shell_env_list *new_list = create_shell_env_list();

    if(!ush->env)
        return ush->env = new_list;

    t_shell_env_list *list_index = ush->env;

    while(list_index->next_list)
        list_index = list_index->next_list;

    list_index->next_list = new_list;

    return new_list;
}

void init_shell_env(t_ush *ush)
{
    struct passwd *passwd = getpwuid(getuid());
    extern char **environ;
    char *pwd = getcwd(NULL, 0);
    char *home = passwd->pw_dir;
    char *login = getlogin();
    char *shell = mx_strjoin(pwd, "/ush");

    create_shell_variable(ush, "PWD",     pwd,   1);
    create_shell_variable(ush, "OLDPWD",  pwd,   1);
    create_shell_variable(ush, "HOME",    home,  1);
    create_shell_variable(ush, "LOGNAME", login, 1);
    create_shell_variable(ush, "USER",    login, 1);

    if(strcmp(getenv("SHELL"), shell))
        create_shell_variable(ush, "SHLVL", "1", 1);
    else {
        char *level = mx_itoa(mx_atoi(getenv("SHLVL")) + 1);
        create_shell_variable(ush, "SHLVL", level, 1);
        mx_strdel(&level);
    }

    create_shell_variable(ush, "SHELL", shell, 1);
    mx_strdel(&shell);
    // print_shell_env(ush);
}

void del_shell_env(t_shell_env_list *head)
{
    t_shell_env_list *list_index;

    for(; head; ) {
        list_index = head;
        head = list_index->next_list;
        mx_strdel(&list_index->variable);
        mx_strdel(&list_index->value);
        list_index->exp = 0;
        list_index->next_list = NULL;
        free(list_index);
        list_index = NULL;
    }
}

static t_shell_env_list *shell_list_address(t_shell_env_list *head, char *variable) 
{
    for(t_shell_env_list *list_index = head; list_index; list_index = list_index->next_list) {
        if(!strcmp(list_index->variable, variable))
            return list_index;
    }

    return NULL;
}

/* allocate memoty */
char *get_shell_variable(t_ush *ush, char *variable) 
{    
    t_shell_env_list *list_index = shell_list_address(ush->env, variable);

    if(list_index)
        return mx_strdup(list_index->value);

    return NULL;
}

void create_shell_variable(t_ush *ush, char *variable, char *value, bool export)
{
    t_shell_env_list *list_index;
    
    if(!(list_index = shell_list_address(ush->env, variable))) {
        list_index = new_shell_env_list(ush);
        list_index->variable = mx_strdup(variable);
    }

    list_index->value = mx_strdup(value);
    list_index->exp = export;
    
    if(export)
        setenv(variable, value, 1);
}

void set_shell_variable(t_ush *ush, char *variable, char *value)
{
    t_shell_env_list *list_index;
    
    if(!(list_index = shell_list_address(ush->env, variable))) {
        list_index = new_shell_env_list(ush);
        list_index->variable = mx_strdup(variable);
    }

    list_index->value = mx_strdup(value);
    
    if(list_index->exp)
        setenv(variable, value, 1);
}

void del_shell_variable(t_ush *ush, char *variable)
{
    t_shell_env_list *previous_list = NULL;
    t_shell_env_list *search_list = NULL;

    for(search_list = ush->env; search_list && strcmp(search_list->variable, variable); 
        search_list = search_list->next_list)
            previous_list = search_list;

    if(!search_list)
        return;

    if(!previous_list)
        ush->env = search_list->next_list;
    else
        previous_list->next_list = search_list->next_list;

    mx_strdel(&search_list->variable);
    mx_strdel(&search_list->value);
    search_list->next_list = NULL;
    search_list->exp = 0;
    free(search_list);
    search_list = NULL;
}

void export_shell_variable(t_ush *ush, char *variable)
{
    t_shell_env_list *list = shell_list_address(ush->env, variable);
    
    if(list) {
        list->exp = 1;
        setenv(list->variable, list->value, 1);
    }
}

void unset_shell_variable(t_ush *ush, char *variable)
{
    t_shell_env_list *list = shell_list_address(ush->env, variable);
    
    if(list) {
        unsetenv(variable);
        del_shell_variable(ush, variable);
    }
}

void print_shell_env(t_ush *ush)
{
    for(t_shell_env_list *list_index = ush->env; list_index; ) {
        printf("%s=%s\n", list_index->variable, list_index->value);
        list_index = list_index->next_list;
    }
}
