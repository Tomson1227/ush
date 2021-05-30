#include "ush.h"

static size_t count_args(char *line)
{
    size_t count = 0;
    uint32_t index = 0, last_index = 0;

    for(; line[index]; ) {

        if(line[index] == ' ') {
            ++count;
            
            while(line[++index] == ' ');
            
            last_index = index;
        }
        else if(line[index] == '\"') {
            if(last_index != index)
                ++count;

            last_index = ++index;

            while(line[index++] != '\"');

            if(last_index != index)
                ++count;

            last_index = index;
        }
        else
            ++index;
    }
    
    if(last_index != index)
        ++count;

    return count;
}

static void separete_args(t_ush *ush, char *line)
{
    uint32_t start = 0, word_index = 0, index = 0;
    size_t len = 0;

    if(!(ush->args = (char **) calloc(count_args(line) + 1, sizeof(char*))))
        perror("ush: ");

    for(; line[index]; ) {
        if(line[index] == ' ') {       
            if((len = index - start)) { 
                ush->args[word_index++] = mx_strndup(&line[start], len);
                ush->args[word_index] = NULL;
            }
            
            while(line[++index] == ' ');

            start = index;
        }
        else if(line[index] == '\"') {
            if((len = index - start)) { 
                ush->args[word_index++] = mx_strndup(&line[start], len);
                ush->args[word_index] = NULL;
            }

            start = ++index;

            while(line[index++] != '\"');

            if((len = index - start - 1)) {
                ush->args[word_index++] = mx_strndup(&line[start], len);
                ush->args[word_index] = NULL;
            }

            start = index;
        }
        else
            ++index;
    }

    if((len = index - start)) {
        ush->args[word_index++] = mx_strndup(&line[start], len);
        ush->args[word_index] = NULL;
    }
}

void replace_str(char *src, char *start_point, size_t length, char *str)
{
    size_t str_length = strlen(str);
    size_t new_length = strlen(src) - length + str_length + 1;
    
    if(malloc_size(src) < new_length) {
        if(!(src = (char *) realloc(src, new_length)))
            perror("ush: ");
    }

    size_t index;
    int move = str_length - length;

    if(move < 0) {
        for(index = length; start_point[index]; ++index)
            start_point[index + move] = start_point[index];  

        start_point[index + move] = '\0';
    }
    else if(move > 0) {
        for(index = strlen(start_point); index >= length; --index) {
            start_point[index + move] = start_point[index];
        }
    }

    memcpy(start_point, str, str_length);
} 

static char *get_param(char *line)
{
    char *param;
    size_t index = 0;

    for(; line[index] && line[index] != '}'; ++index);

    param = mx_strndup(line, index);

    for(index = 0; param[index]; ++index) {
        if(!mx_isalpha(param[index]) && !mx_isdigit(param[index])) {
            write(STDERR_FILENO, "ush: ", 5);
            write(STDERR_FILENO, param, strlen(param));
            write(STDERR_FILENO, ": bad substitution\n", 19);
            return NULL;
        }
    }

    return param;
}

char *get_env_value(char *env_key) 
{
    char *value;
    int env_value_len = strlen(env_key);

    for(char **env = environ; *env != 0; env++) {
        if(!strncmp(env_key, *env, env_value_len) && (*env)[env_value_len] == '=') {
            value = (char*) calloc(strlen(&(*env)[env_value_len + 1]), sizeof(char));
            strcpy(value, &(*env)[++env_value_len]);
            return value;
        }
    }

    return NULL;    // if not find return NULL
}

static char *get_command(char *str)
{
    uint8_t count = 1;
    size_t index = 2;
    
    for(; count; ++index) {
        if(str[index] == '(')
            ++count;
        else if(str[index] == ')')
            --count;
    }

    return mx_strndup(&str[2], index - 3);
}

static char *process_command(char *command)
{
    char *result = command;

    return result;
}

static void replace_params(char *line) {
    int index;

    while((index = mx_get_substr_index(line, "${")) >= 0) {
        char *param, *replace;
        param = get_param(&line[index + 2]);
        replace  = get_env_value(param);
        replace_str(line, &line[index], strlen(param) + 3, replace);
        mx_strdel(&param);
        mx_strdel(&replace);
    }
}

static void replace_math(char *line) {
    int index;

    while((index = mx_get_substr_index(line, "$((")) >= 0) {
        char *param, *replace;
        param = get_param(&line[index + 2]);
        replace  = get_env_value(param);
        replace_str(line, &line[index], strlen(param) + 3, replace);
        mx_strdel(&param);
        mx_strdel(&replace);
    }
}

void replace_command(char *line)
{
    int index;
    
    while((index = mx_get_substr_index(line, "$(")) >= 0) {
        char *command = get_command(&line[index]);
        char *result = process_command(command);
        replace_str(line, &line[index], strlen(command) + 3, result);
        index += strlen(command) + 3;
        mx_strdel(&command);
    }
} 

void validate_args(t_ush *ush, t_line *line)
{
    char *clean_line = mx_strtrim(line->line);
    replace_params(clean_line);
    replace_math(clean_line);
    replace_command(clean_line);
    separete_args(ush, clean_line);
}
