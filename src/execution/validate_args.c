#include "ush.h"

static void replace_params(char **clean_line);
static void replace_math(char **clean_line);
static void replace_command(char **clean_line);
static void separete_args(t_ush *ush, char *line);

void validate_args(t_ush *ush, char *line)
{
    char *clean_line = mx_strtrim(line);
    replace_params(&clean_line);
    replace_math(&clean_line);
    replace_command(&clean_line);
    separete_args(ush, clean_line);
}

static size_t count_line_args(char *line)
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

    if(!(ush->args = (char **) calloc(count_line_args(line) + 1, sizeof(char*))))
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

void replace_str(char **src, size_t start_index, size_t length, char *str)
{
    size_t str_length = (str == NULL) ? 0 : strlen(str);
    size_t new_length = strlen(*src) - length + str_length;

    if(malloc_size(*src) < new_length) {
        if(!(*src = (char *) realloc(*src, new_length)))
            strerror(errno);
    }

    size_t index;
    int move = str_length - length;

    if(move < 0) {
        for(index = start_index + str_length; index < strlen(*src); ++index)
            (*src)[index] = (*src)[index - move];  
    }
    else if(move > 0) {
        for(index = strlen(*src); index >= start_index + length; --index) {
            (*src)[index + move] = (*src)[index];
    
            if(!index) break;
        }
    }

    memcpy(&(*src)[start_index], str, str_length);
} 

static char *get_quote_content(char *line, char quotes_open, char quotes_close)
{
    if(!(*line) || *line != quotes_open)
        return NULL;

    size_t index = 1;

    for(uint16_t count = 1; line[index] && count; ++index) {
        if(line[index] == quotes_open)
            ++count;
        else if(line[index] == quotes_close)
            --count;
    }   

    return mx_strndup(&line[1], index - 2);
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

    return NULL;
}

static char *process_command(char *command)
{
    char *result = mx_strnew(0);
    int index = 0;

    FILE *fp = popen(command, "r");

    if(fp == NULL)
        perror("Failed to run command\n");

    while(fgets(&result[index], malloc_size(result) - strlen(result), fp) != NULL) {
        index = strlen(result);
        result = (char *) realloc(result, index + 24);
    }

    if(result[index - 1] == '\n')
        result[index - 1] = '\0'; 
        
    pclose(fp);
    
    return result;
}

static void replace_params(char **clean_line) 
{
    char *line = *clean_line;
    int index = 0;

    while((index = mx_get_char_index(&line[index], '$')) >= 0) {
        char *param, *replace;
        size_t param_length;

        if(line[index + 1] == '{') {
            param = get_quote_content(&line[index + 1], '{', '}');
            param_length = strlen(param) + 3;
        }
        else if(line[index + 1] != '(') {
            param = get_quote_content(&line[index + 1], '\0', ' ');
            param_length = strlen(param) + 1;
        }
        else {
            ++index;
            continue;
        }
        
        for(size_t i = 0; param[i]; ++i) {
            if(!mx_isalpha(param[i]) && !mx_isdigit(param[i])) {
                fprintf(stderr, "ush: %s: bad substitution\n", param);
                // replace_str(clean_line, index, strlen(param) + 3, "");
                return;
            }
        }

        replace  = get_env_value(param);
        replace_str(clean_line, index, strlen(param) + 3, replace);
        mx_strdel(&replace);
        mx_strdel(&param);

    }

    while((index = mx_get_substr_index(&line[index], "${")) >= 0) {
        char *param, *replace;

        param = get_quote_content(&line[index + 1], '{', '}');
        
        for(size_t i = 0; param[i]; ++i) {
            if(!mx_isalpha(param[i]) && !mx_isdigit(param[i])) {
                fprintf(stderr, "ush: %s: bad substitution\n", param);
                // replace_str(clean_line, index, strlen(param) + 3, "");
                return;
            }
        }

        replace  = get_env_value(param);
        replace_str(clean_line, index, strlen(param) + 3, replace);
        mx_strdel(&replace);
        mx_strdel(&param);
    }
}

static char *math_execute(char *str) 
{
    char path[24];
    char *command = mx_strnew(strlen(str) + 8);
    sprintf(command, "echo $(%s)", str);
    FILE *fp = popen(command, "r");

    if(fp == NULL)
        perror("Failed to run command\n");

    while(fgets(path, sizeof(path), fp) != NULL);
        
    pclose(fp);
    
    for(uint8_t i = 0; path[i];++i) {
        if(mx_isdigit(path[i]))
            continue;
        else {
            path[i] = '\0';
            break;
        }
    }
    
    return mx_strdup(path);
}

static void replace_math(char **clean_line) 
{
    char *line = *clean_line;
    int index;

    while((index = mx_get_substr_index(line, "$((")) >= 0) {
        char *param, *replace;
        param = get_quote_content(&line[index + 1], '(', ')');
        replace  = math_execute(param);
        replace_str(clean_line, index, strlen(param) + 3, replace);
        mx_strdel(&param);
    }
}

static void replace_command(char **clean_line)
{
    char *line = *clean_line;
    int index;
    
    while((index = mx_get_substr_index(line, "$(")) >= 0) {
        char *command = get_quote_content(&line[index + 1], '(', ')');
        char *result = process_command(command);
        replace_str(clean_line, index, strlen(command) + 3, result);
        index += strlen(command) + 3;
        mx_strdel(&command);
    }
} 
