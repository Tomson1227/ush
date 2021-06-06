#include "ush.h"

static void disable_special_symbols(char **args);

/*
-n не выводить в конце символ новой строки
-e включить интерпретацию управляющих символов (default)
-E отключить интерпретацию управляющих символов
*/
void echo_func(t_ush *ush, t_process *process)
{
    uint32_t options = get_opt(process->args, "neE");
    uint32_t index = start_index(&process->args[1]) + 1;
    uint8_t opt_n = READ_OPT(options, 1);
    uint8_t opt_e = READ_OPT(options, 2);
    uint8_t opt_E = READ_OPT(options, 4);

    if(!opt_E)
        disable_special_symbols(&process->args[index]);

    if(process->args[index])
        printf("%s", process->args[index++]);

    for(; process->args[index]; ++index)
        printf("%s ", process->args[index]);

    if(!opt_n)
        printf("\n");

    process->status = 0;
}

static void disable_special_symbols(char **args)
{
    int i = 0;

    for(uint32_t index = 0; args[index]; ++index) {
        i = 0;
        while((i = mx_get_substr_index(&args[index][i], "\\a")) >= 0)
            replace_str(&args[index], i++, 2, "\a");
        i = 0;
        while((i = mx_get_substr_index(&args[index][i], "\\b")) >= 0)
            replace_str(&args[index], i++, 2, "\b");
        i = 0;
        while((i = mx_get_substr_index(&args[index][i], "\\t")) >= 0)
            replace_str(&args[index], i++, 2, "\t");
        i = 0;
        while((i = mx_get_substr_index(&args[index][i], "\\n")) >= 0)
            replace_str(&args[index], i++, 2, "\n");
        i = 0;
        while((i = mx_get_substr_index(&args[index][i], "\\v")) >= 0)
            replace_str(&args[index], i++, 2, "\v");
        i = 0;
        while((i = mx_get_substr_index(&args[index][i], "\\f")) >= 0)
            replace_str(&args[index], i++, 2, "\f");
    }
}

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
