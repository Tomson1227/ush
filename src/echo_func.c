#include "ush.h"

void echo_func(t_main *interface)
{
    interface->status = 0;
}

// typedef struct s_echo_func {
//     bool flag_n;
//     bool flag_e;
//     uint8_t index;
// }      t_echo_func;

// static inline void read_flags(t_main *interface, t_echo_func *flags)
// {
//     for(; flags->index < interface->func_arg.number; flags->index) {
//         if(!mx_strncmp(interface->func_arg.value[flags->index], "-", 1)) {
//             for(int j = 1; interface->func_arg.value[flags->index][j]; ++j) {
//                 if(interface->func_arg.value[flags->index][j] == 'n')
//                     flags->flag_n = 0;
//                 else if(interface->func_arg.value[flags->index][j] == 'e')
//                     flags->flag_e = 1;
//                 else if(interface->func_arg.value[flags->index][j] == 'E')
//                     flags->flag_e = 0;
//             }
//         }
//         else
//             break;
//     }
// }

// static inline t_echo_func *init_echo_struct(void)
// {
//     t_echo_func *echo_struct;
//     echo_struct->flag_n = 1;
//     echo_struct->flag_e = 0;
//     echo_struct->index = 1;

//     return echo_struct;
// }

// static void text_modify(char *text)
// {
//     for(size_t i = 0; text[i]; ++i) {

//     }
// }

// void echo_func(t_main *interface)
// {
//     t_echo_func *flags = init_echo_struct();

//     read_flags(interface, flags);
    
//     while(interface->func_arg.value[flags->index]) {
//         if(flags->flag_e)
//             text_modify(inteinterface->func_arg.value[flags->index]);

//         echo_result(interface->func_arg.value[flags->index]);
//     }

//     if(flags->flag_n)
//         mx_printchar('\n');

//     interface->status = 1;
// }

// //-n не выводить в конце символ новой строки
// //-e включить интерпретацию управляющих символов, перечисленных ниже
// //-E отключить интерпретацию ткаих управляющих символов в СТРОКах

// /*
// \NNN символ с ASCII кодом NNN (восьмеричное)
// \\ обратная косая черта
// \a тревога (BEL)
// \b забой
// \c подавлять символ новой строки в конце
// \f перевод страницы
// \n новая строка
// \r возврат каретки
// \t горизонтальаня табуляция
// \v вертикальаня табуляция
// */
