#include "ush.h"

//temp struct
typedef struct s_echo_func {
    bool flag_n;
    bool flag_e;
    uint8_t offset;
}      t_echo_func;

static inline void read_flags(t_main *interface, t_echo_func *flags)
{
    while(!mx_strncmp(interface->func_arg.value[flags->offset], "-", 1)) {
        for(int j = 1; interface->func_arg.value[flags->offset][j]; ++j) {
            if(interface->func_arg.value[flags->offset][j] == 'n')
                flags->flag_n = 0;
            else if(interface->func_arg.value[flags->offset][j] == 'e')
                flags->flag_e = 1;
            else if(interface->func_arg.value[flags->offset][j] == 'E')
                flags->flag_e = 0;

            ++flags->offset;
        }
    }
}

static inline t_echo_func *init_echo_struct(void)
{
    t_echo_func *echo_struct = (t_echo_func *) calloc(1, sizeof(t_echo_func));
    echo_struct->flag_n = 1;
    echo_struct->flag_e = 0;
    echo_struct->offset = 1;

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
    char *new_str = NULL;

    if(!count)
        return;

    if(!(new_str = calloc(mx_strlen(*text) + count, sizeof(char))))
        strerror(errno);

    for(size_t i = 0, j = 0; *text[i]; ++i, ++j) {
        if((*text[i] >= 0x7 && *text[i] <= 0xD)) {
            new_str[j++] = '\\';
            new_str[j] = get_excape(*text[i]);
        }
        else
            new_str[j] = *text[i];
    }

    free(*text);
    *text = new_str;
}

void echo_func(t_main *interface)
{
    t_echo_func *flags = init_echo_struct();
    read_flags(interface, flags);
    
    if(flags->flag_e) {
        for(uint8_t index = 0; interface->func_arg.value[index];) {
            text_modify(&interface->func_arg.value[index]);
        }
    }

    if(flags->flag_n)
        add_newline(&interface->func_arg.value[interface->func_arg.number - 1]);

    copy_args(&interface->result, &interface->func_arg);
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
