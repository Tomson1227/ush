#include "ush.h"

static char *get_last_arg(t_line *line)
{
    char *search_last = NULL;
    uint16_t line_index = 0;
    size_t size = 0;
    line->tab_func->command = true;

    if(!line->line || strlen(line->line) == 0)
        return mx_strnew(0);

    while(line->line[line_index + size]) {
        while(line->line[line_index] == ' ') 
            ++line_index;
        
        while(line->line[line_index + size] != ' '
           && line->line[line_index + size])
            ++size;

        if(line->line[line_index + size]) {
            line_index += size;
            size = 0;
            line->tab_func->command = false;
        }
    }

    if(!line->tab_func->last_arg || line->tab_func->last_arg[0] != 0x1B)
        line->tab_func->line_index = line_index;

    if(size)
        return mx_strdup(&(line->line[line_index]));
    else
        return mx_strnew(0);
}

static void search_command(t_line *line, t_tab_func *tab_func)
{
    DIR *dir = NULL;
    struct dirent *dir_param;
    uint16_t count_names = 0;

    for(uint8_t index = 0; tab_func->bin_dirs[index]; ++index) {
        if(!(dir = opendir(tab_func->bin_dirs[index])))
            continue;

        while((dir_param = readdir(dir))) {
            if(!mx_strncmp(tab_func->last_arg, dir_param->d_name, mx_strlen(tab_func->last_arg)) || 
               !mx_strlen(tab_func->last_arg))
                ++count_names;
        }

        closedir(dir);
    }

    tab_func->var_num = count_names;
    if((tab_func->variants = (char **) calloc(count_names + 1, sizeof(char *))))
        strerror(errno);

    count_names = 0;  
    tab_func->variants[count_names] = NULL;

    for(uint8_t index = 0; tab_func->bin_dirs[index]; ++index) {
        if(!(dir = opendir(tab_func->bin_dirs[index]))) {
            write(STDERR_FILENO, "ush: ", 5);
            strerror(errno);
            continue;
        }    

        while((dir_param = readdir(dir))) {
            if(!mx_strncmp(tab_func->last_arg, dir_param->d_name, mx_strlen(tab_func->last_arg)) || 
               !mx_strlen(tab_func->last_arg)) {
                tab_func->variants[count_names++] = mx_strdup(dir_param->d_name);
                tab_func->variants[count_names] = NULL;
            }
        }

        closedir(dir);
    }
}

static void devide_path_file(t_tab_func *tab_func)
{
    DIR *dir = NULL;
    size_t position = 0, index = 0;

    if((!tab_func->last_arg) || !mx_strcmp(tab_func->last_arg, "\0")) {
        tab_func->path = "./";
        tab_func->serch_arg = "\0";
        return;
    }
    
    while((index = mx_get_char_index(&tab_func->last_arg[index], '/')) != -1)
        position = ++index;

    if(position) {
        tab_func->path = mx_strjoin("./", tab_func->last_arg);
        tab_func->path[position + 2] = '\0';  
        if((dir = opendir(tab_func->path))) {
            closedir(dir);
            tab_func->line_index += position;

            tab_func->serch_arg = mx_strdup(&tab_func->last_arg[position]);
            return;
        }

        free(tab_func->path);
        closedir(dir);
    }

    tab_func->path = mx_strdup("./");
    tab_func->serch_arg = mx_strdup(tab_func->last_arg);
}

static void search_file(t_line *line, t_tab_func *tab_func)
{
    DIR *dir = NULL;
    uint16_t count = 0;
    struct stat dir_stat;
    struct dirent *dir_param = NULL;
    char *path_name = NULL;
    
    devide_path_file(line->tab_func);
    dir = opendir(tab_func->path);

    while((dir_param = readdir(dir))) {
        if((!mx_strncmp(tab_func->serch_arg, dir_param->d_name, mx_strlen(tab_func->serch_arg)) || 
            !mx_strlen(tab_func->serch_arg)) && mx_strncmp(dir_param->d_name, ".", 1))
            ++count;
    }
    
    tab_func->var_num = count;

    if((tab_func->variants = (char **) calloc(count + 1, sizeof(char *))))
        strerror(errno);

    dir = opendir(tab_func->path);
    count = 0;  
    tab_func->variants[count] = NULL;

    while((dir_param = readdir(dir))) {
        if((!mx_strncmp(tab_func->serch_arg, dir_param->d_name, mx_strlen(tab_func->serch_arg)) || 
            !mx_strlen(tab_func->serch_arg)) && mx_strncmp(dir_param->d_name, ".", 1)) {
            path_name = mx_strjoin(tab_func->path, dir_param->d_name);
            stat(path_name, &dir_stat); 
            
            if(S_ISDIR(dir_stat.st_mode)) {
                tab_func->variants[count++] = mx_strjoin(dir_param->d_name, "/");
                tab_func->variants[count] = NULL;
            }
            else {
                tab_func->variants[count++] = mx_strdup(dir_param->d_name);
                tab_func->variants[count] = NULL;
            }

            free(path_name);
            path_name = NULL;
        }
    }

    closedir(dir);
}

static void print_column(t_tab_func *tab_fuck) 
{
    struct winsize width;
    const uint8_t spase_num = 3;
    uint32_t max_size = 0, count_words = 0, count_rows = 0;

    for(uint16_t index = 0; tab_fuck->variants[index]; ++index) {
        uint32_t size = mx_strlen(tab_fuck->variants[index]);
        ++count_words;

        if(size > max_size)
            max_size = size;
    }

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &width);
    uint8_t column_num = width.ws_col / (max_size + spase_num);
    mx_printchar('\n');
    ERASE_DOWN;

    for(int16_t index = 0; tab_fuck->variants[index]; ++index) { 
        if(!(index % column_num) && index) {
            mx_printchar('\n');
            ++count_rows;
            }

        if(!mx_strcmp(tab_fuck->last_arg, tab_fuck->variants[index]))
            mx_printstr(INVERT_COLOR);

        mx_printstr(tab_fuck->variants[index]);
        
        if(!mx_strcmp(tab_fuck->last_arg, tab_fuck->variants[index]))
            mx_printstr(RESET_ALL);

        uint32_t size = mx_strlen(tab_fuck->variants[index]);
        
        for(uint32_t size = mx_strlen(tab_fuck->variants[index]); size < max_size + spase_num; ++size) {
            mx_printchar(' ');
        }
    }

    CURSOR_N_UP(count_rows + 1);
}

static void line_autocomplete(t_line *line)
{
    mx_strcpy(&line->line[line->tab_func->line_index], line->tab_func->variants[line->tab_func->var_index++]);
    line->position = mx_strlen(line->line);

    free(line->tab_func->last_arg);
    line->tab_func->last_arg = get_last_arg(line);

    if(line->tab_func->var_index >= line->tab_func->var_num)
        line->tab_func->var_index = 0;
}

static void add_char(char *str, char c)
{
    size_t last_index = strlen(str);
    str[last_index++] = c;
    str[last_index] = '\0';
}

void autocomplete(t_line *line)
{   
    char *last_arg = get_last_arg(line);

    if(!line->tab_func->last_arg || mx_strcmp(line->tab_func->last_arg, last_arg)) {
        free(line->tab_func->last_arg);
        line->tab_func->var_index = 0;
        line->tab_func->last_arg = last_arg;
        line->tab_func->command ? search_command(line, line->tab_func) : 
                                  search_file(line, line->tab_func);
    }
    else {
        mx_strdel(&last_arg);
        line_autocomplete(line);
    }

    if(line->tab_func->var_num > 1) {
        reset_line(line);
        print_column(line->tab_func);
    }
    else if(line->tab_func->var_num){
        line_autocomplete(line);
        add_char(line->line, ' ');
        line->tab_func->last_arg[0] = 0x1B;
        ERASE_DOWN;
    }
    reset_cursore_position(line);
}
