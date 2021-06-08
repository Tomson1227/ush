#include "ush.h"

static void separete_args(t_ush *ush, char *line);
static void replace_variables(t_ush *ush, char **clean_line);
static void replace_math(char **clean_line);
static void replace_command(char **clean_line);
static void replace_special_symbols(t_ush *ush, char **clean_line);

void replace_args_escapes(t_ush *ush, t_process *procass)
{
    for(size_t index = 0; procass->args[index]; ++index) {
        replace_variables(ush ,&procass->args[index]);
        replace_math(&procass->args[index]);
        replace_command(&procass->args[index]);
        replace_special_symbols(ush, &procass->args[index]);
    }
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

void validate_args(t_ush *ush, char *line)
{
    uint32_t start = 0, word_index = 0, index = 0;
    size_t len = 0;

    char *clean_line = mx_strtrim(line);

    if(!(ush->args = (char **) calloc(count_line_args(clean_line) + 1, sizeof(char*))))
        perror("ush: ");

    for(; clean_line[index]; ) {
        if(clean_line[index] == ' ') {       
            if((len = index - start)) { 
                ush->args[word_index++] = mx_strndup(&clean_line[start], len);
                ush->args[word_index] = NULL;
            }
            
            while(clean_line[++index] == ' ');

            start = index;
        }
        else if(clean_line[index] == '\"') {
            if((len = index - start)) { 
                ush->args[word_index++] = mx_strndup(&clean_line[start], len);
                ush->args[word_index] = NULL;
            }

            start = ++index;

            while(clean_line[index++] != '\"');

            if((len = index - start - 1)) {
                ush->args[word_index++] = mx_strndup(&clean_line[start], len);
                ush->args[word_index] = NULL;
            }

            start = index;
        }
        else
            ++index;
    }

    if((len = index - start)) {
        ush->args[word_index++] = mx_strndup(&clean_line[start], len);
        ush->args[word_index] = NULL;
    }
}

//Debug
static void debug_print(char *str, size_t len)
{
    for(size_t i = 0; i <= len; ++i) {
        if(str[i] >= ' ' && str[i] <= '~')
            printf("%c", str[i]);
        else
            printf("\'%d\'", str[i]);
    }
    printf("\n");
}

void replace_str(char **src, size_t start_index, size_t length, char *str)
{
    size_t index;
    size_t str_length = (str == NULL) ? 0 : strlen(str);
    size_t new_length = strlen(*src) - length + str_length;
    int move = str_length - length;

    if(malloc_size(*src) < new_length) {
        if(!(*src = (char *) realloc(*src, new_length)))
            strerror(errno);
    }

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

    memcpy(&((*src)[start_index]), str, str_length);
} 

static char *get_quote_content(char *line, char quotes_open, char quotes_close)
{
    if(!(*line))
        return NULL;

    size_t start_index, end_index;
    size_t line_length = strlen(line);
    uint32_t count = 1;

    for(start_index = 0; line[start_index] && line[start_index]!= quotes_open; ++start_index);

    (start_index == line_length) ? start_index = 0 : ++start_index;

    for(end_index = start_index; line[end_index] && count; ++end_index) {
        (line[end_index] == quotes_open) ? ++count : 
        (line[end_index] == quotes_close) ? --count : count;
    }   

    (end_index + start_index == line_length) ? end_index = line_length : --end_index;

    return mx_strndup(&line[start_index], end_index - start_index);
}

char *get_env_value(t_ush *ush, char *variable) 
{
    char *value = NULL;

    if(!(value = getenv(variable)))
        value = get_shell_variable(ush, variable);

    return value;
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

static void replace_variables(t_ush *ush, char **clean_line) 
{
    int index;

    while((index = mx_get_substr_index(*clean_line, "${")) >= 0) {
        char *variable, *replace;
        size_t variable_length;

        variable = get_quote_content(&(*clean_line)[index], '{', '}');
        variable_length = strlen(variable) + 3;
        
        if(!strcmp(variable, "?")) {
            replace = mx_itoa(ush->local_status);
            replace_str(clean_line, index, variable_length, replace);
            mx_strdel(&variable);
            continue;
        }

        for(size_t i = 0; variable[i]; ++i) {
            if(!mx_isalpha(variable[i]) && !mx_isdigit(variable[i])) {
                fprintf(stderr, "ush: %s: bad substitution\n", variable);
                replace_str(clean_line, index, variable_length, "");
                return;
            }
        }

        replace  = get_env_value(ush, variable);
        replace_str(clean_line, index, variable_length, replace);
        mx_strdel(&variable);
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
    int index;

    while((index = mx_get_substr_index(*clean_line, "$((")) >= 0) {
        char *variable, *replace;
        variable = get_quote_content(&(*clean_line)[index], '(', ')');
        replace  = math_execute(variable);
        replace_str(clean_line, index, strlen(variable) + 3, replace);
        mx_strdel(&variable);
    }
}

static void replace_command(char **clean_line)
{
    char *line = *clean_line;
    int index;
    
    while((index = mx_get_substr_index(line, "$(")) >= 0) {
        char *command = get_quote_content(&line[index], '(', ')');
        char *result = process_command(command);
        replace_str(clean_line, index, strlen(command) + 3, result);
        mx_strdel(&command);
    }
} 

static char *separete_variable(char *line)
{
    if(!(*line))
        return NULL;

    size_t end_index;
    size_t line_length = strlen(line);
    uint32_t count = 1;

    for(end_index = 0; line[end_index] && count; ++end_index)
        count -= (line[end_index] == ' '  || 
                  line[end_index] == '\'' || 
                  line[end_index] == '\"');

    count ? end_index = line_length : --end_index;

    return mx_strndup(line, end_index);
}

static void replace_special_symbols(t_ush *ush, char **clean_line)
{
    char *variable, *replace;
    int tilda_index, variable_index;
    
    if((tilda_index = mx_get_char_index(*clean_line, '~')) >= 0)
        replace_str(clean_line, tilda_index, 1, ush->home);

    while((variable_index = mx_get_char_index(*clean_line, '$')) >= 0) {
        variable = separete_variable(&(*clean_line)[variable_index + 1]);

        if(!strcmp(variable, "?")) {
            replace = mx_itoa(ush->local_status);
            replace_str(clean_line, variable_index, strlen(variable) + 1, replace);
            mx_strdel(&variable);
            continue;
        }

        for(size_t i = 0; variable[i]; ++i) {
            if(!mx_isalpha(variable[i]) && !mx_isdigit(variable[i])) {
                fprintf(stderr, "ush: %s: bad substitution\n", variable);
                replace_str(clean_line, variable_index, strlen(variable) + 1, "");
                return;
            }
        }

        replace  = get_env_value(ush, variable);
        replace_str(clean_line, variable_index, strlen(variable) + 1, replace);
        mx_strdel(&variable);
    }
}
