#include "ush.h"

typedef struct s_tab_func {
    uint16_t tab_press;
    char **variants;
    char *last_arg;
    char *serch_arg;
}              t_tab_func;

static inline t_tab_func *init_tab_struct(void)
{
    t_tab_func *tab_func = NULL;
    
    if(!(tab_func = (t_tab_func *) calloc(1, sizeof(t_tab_func))))
        strerror(errno);

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
        while(line->line[count] == ' ') ++count;
        while(line->line[count + size] != ' ' || line->line[count + size] != '\0') ++size;
        if(line->line[count + size] == ' ') size = 0;
    }

    tab_funk->serch_arg = mx_strndup(&line->line[count], size);
}

static void search_familiar(t_line *line, t_tab_func *tab_funk)
{
    DIR *dir = opendir(line->prompt);
    uint16_t count = 0;
    struct dirent *dir_param;
    
    while((dir_param = readdir(dir))) {
        if(!mx_strncmp(tab_funk->last_arg, dir_param->d_name, mx_strlen(tab_funk->last_arg)))
        ++count;
    }

    closedir(dir);
    

    if((tab_funk->variants = (char **) calloc(count + 1, sizeof(char *))))
        strerror(errno);

    dir = opendir(line->prompt);
    tab_funk->variants[count] = NULL;
    count = 0;  

    while((dir_param = readdir(dir))) {
        if(!mx_strncmp(tab_funk->last_arg, dir_param->d_name, mx_strlen(tab_funk->last_arg))) {
            tab_funk->variants[count++] = mx_strdup(dir_param->d_name);
            tab_funk->variants[count] = NULL;
        }
    }

    closedir(dir);
}

void auto_completion(t_line *line)
{
    t_tab_func *tab_funk = init_tab_struct();
    get_last_arg(line, tab_funk);
    search_familiar(line, tab_funk);

}
