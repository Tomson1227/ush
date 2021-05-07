#ifndef PROCESS_H
#define PROCESS_H //#include "process.h"

/*PROCESS register 

process->control
---------------------------------------------------------------------------------
| 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
---------------------------------------------------------------------------------

pipe mod [1:0]:
    * 0 | 0 - unused (default)
    * 0 | 1 - read pipe
    * 1 | 0 - write to pipe
    * 1 | 1 - r/w pipe

result mod [3:2]:
    * 0 | 0 - print result to stdout (default)
    * 0 | 1 - print result to pipe
    * 1 | 0 - write/create file and set result
    * 1 | 1 - overwrite/create file and set result

logical mod [5:4]:
    * 0 | X - lodical mod OFF (default)
    * 1 | 0 - begin process if status of previous if false
    * 1 | 1 - begin process if status of previous if true

*/

#define PIPE_READ   0x0001U     // 0b 0000 0000 0000 0001
#define PIPE_WRITE  0x0002U     // 0b 0000 0000 0000 0010
#define PIPE_R_W    0x0003U     // 0b 0000 0000 0000 0011

#define PRINT_PIPE      0x0006U  // 0b 0000 0000 0000 0110
#define WRITE_FILE      0x0008U  // 0b 0000 0000 0000 1000
#define OVERWRITE_FILE  0x000CU  // 0b 0000 0000 0000 1100

#define LOGIC_OR        0X0020U  // 0b 0000 0000 0010 0000
#define LOGIC_AND       0x0030U  // 0b 0000 0000 0011 0000

#endif /* PROCESS */
