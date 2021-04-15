#include "ush.h"

//temp struct
typedef struct s_echo_func {
    bool flag_n;
    bool flag_e;
    uint8_t index;
}      t_echo_func;

static inline void read_flags(t_main *interface, t_echo_func *flags)
{
    for(; flags->index < interface->func_arg.number; ++flags->index) {
        if(!mx_strncmp(interface->func_arg.value[flags->index], "-", 1)) {
            for(int j = 1; interface->func_arg.value[flags->index][j]; ++j) {
                if(interface->func_arg.value[flags->index][j] == 'n')
                    flags->flag_n = 0;
                else if(interface->func_arg.value[flags->index][j] == 'e')
                    flags->flag_e = 1;
                else if(interface->func_arg.value[flags->index][j] == 'E')
                    flags->flag_e = 0;
            }
        }
        else
            break;
    }
}

static inline t_echo_func *init_echo_struct(void)
{
    t_echo_func *echo_struct = (t_echo_func *) calloc(1, sizeof(t_echo_func));
    echo_struct->flag_n = 1;
    echo_struct->flag_e = 0;
    echo_struct->index = 1;

    return echo_struct;
}

static size_t count_escapes(char *text)
{
    size_t count = 0;

    for(size_t index = 0; text[index]; ++index) {
        if(text[count] >= 0x7 && text[count] <= 0xD)    
            ++count;
    }

    return count;
}

static char get_excape(char c)
{
    switch (c) {
        case '\a':
            return 'a';
        case '\b':
            return 'b';
        case '\f':
            return 'f';
        case '\n':
            return 'n';
        case '\r':
            return 'r';
        case '\t':
            return 't';
        case '\v':
            return 'v';
        default:
            return c;
    }
}

static void text_modify(char **text)
{

    size_t count = count_escapes(*text);
    
    if(!count)
        return;

    char *new_str = calloc(mx_strlen(*text) + count, sizeof(char));

    for(size_t i = 0, j = 0; *text[i]; ++i, ++j) {
        if((*text[i] >= 0x7 && *text[i] <= 0xD)) {
            new_str[j++] = '\\';
            new_str[j] = get_excape(*text[i]);
        }
        else
            new_str[j] = *text[i];
    }

    free(*text);
    **text = *new_str;
}

void echo_func(t_main *interface)
{
    t_echo_func *flags = init_echo_struct();
    read_flags(interface, flags);

    interface->result.value = (char **) calloc(interface->func_arg.number - flags->index, sizeof(char *));
    
    for(interface->result.number = 0; interface->func_arg.value[flags->index];) {

        if(flags->flag_e)
            text_modify(&interface->func_arg.value[flags->index]);

        interface->result.value[interface->result.number++] = interface->func_arg.value[flags->index++];
        interface->result.value[interface->result.number] = NULL;
    }

    //confused to use flag_n with pipe together

    interface->status = 1;
}

//-n не выводить в конце символ новой строки
//-e включить интерпретацию управляющих символов, перечисленных ниже
//-E отключить интерпретацию ткаих управляющих символов в СТРОКах

/*
\NNN символ с ASCII кодом NNN (восьмеричное)
\\ обратная косая черта
\a тревога (BEL)
\b забой
\c подавлять символ новой строки в конце
\f перевод страницы
\n новая строка
\r возврат каретки
\t горизонтальаня табуляция
\v вертикальаня табуляция
*/
