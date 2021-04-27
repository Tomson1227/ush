#include "ush.h"

typedef struct s_tab_func {
    uint16_t tab_press;
    char **variants;
    char *last_arg;
    char *serch_arg;
    char *path; 
}              t_tab_func;

static inline t_tab_func *init_tab_struct(void)
{
    t_tab_func *tab_func = NULL;
    
    if(!(tab_func = (t_tab_func *) calloc(1, sizeof(t_tab_func))))
        strerror(errno);

    tab_func->variants = NULL;
    tab_func->last_arg = NULL;
    tab_func->serch_arg = NULL;
    tab_func->path = NULL;

    return tab_func;
}

static void get_last_arg(t_line *line, t_tab_func *tab_funk)
{
    if(!line->line || strlen(line->line) == 0) {
        tab_funk->serch_arg = mx_strdup("");
        return;
    }

    char *search_last = NULL;
    size_t count = 0, size = 0;
    while(line->line[count + size] != '\0') {
        while(line->line[count] == ' ') 
            ++count;
        
        // mx_printstr("count = ");
        // mx_printint(count);
        // mx_printchar('\n');

        while(line->line[count + size] != ' ' && line->line[count + size])
            ++size;

        if(line->line[count + size] == ' ') {
            count += size;
            size = 0;
        }

        // mx_printstr("size = ");
        // mx_printint(size);
        // mx_printchar('\n');
    }

    tab_funk->last_arg = mx_strdup(&(line->line[count]));
    // mx_printstr("last arg = ");
    // mx_printstr(tab_funk->last_arg);
    // mx_printchar('\n');
}

static void search_familiar(t_line *line, t_tab_func *tab_funk)
{
    DIR *dir = NULL;

    if(!(dir = opendir(tab_funk->path))) {
        free(tab_funk->path);
        tab_funk->path = ".";
        dir = opendir(tab_funk->path);
    }

    uint16_t count = 0;
    struct dirent *dir_param;
    // mx_printstr("last arg = \n");
    // mx_printstr(tab_funk->last_arg);
    // mx_printchar('\n');
    while((dir_param = readdir(dir))) {
        if(!mx_strncmp(tab_funk->last_arg, dir_param->d_name, mx_strlen(tab_funk->last_arg)))
            ++count;
    }

    closedir(dir);
    
    if((tab_funk->variants = (char **) calloc(count + 1, sizeof(char *))))
        strerror(errno);

    dir = opendir(tab_funk->path);
    count = 0;  
    tab_funk->variants[count] = NULL;

    while((dir_param = readdir(dir))) {
        if(!mx_strncmp(tab_funk->last_arg, dir_param->d_name, mx_strlen(tab_funk->last_arg))) {
            tab_funk->variants[count++] = mx_strdup(dir_param->d_name);
            tab_funk->variants[count] = NULL;
        }
    }

    closedir(dir);
}

static void take_path(t_tab_func *tab_funk)
{
    size_t position = 0;
    size_t index = 0;

    if((!tab_funk->last_arg) || !mx_strcmp(tab_funk->last_arg, ""))
        return;
    
    while((index = mx_get_char_index(&tab_funk->last_arg[index], '/')) != -1)
        position = index++;

    char *path = mx_strndup(tab_funk->last_arg, position);
}

void auto_completion(t_line *line)
{
    t_tab_func *tab_funk = init_tab_struct();
    get_last_arg(line, tab_funk);
    take_path(tab_funk);
    search_familiar(line, tab_funk);

    if(line->tab_var)
        mx_del_strarr(&line->tab_var);

    line->tab_var = tab_funk->variants;
    // deinit_tab_struct(tab_funk);
}
