#include "ush.h"

// // unset
// void unset_name(char *name) {
//     if(unsetenv(name) == -1) 
//         perror("EROR: ");
// }

// // export
// void export_name(char *name) {
//     if(putenv(name) == -1)
//         perror("EROR: ");
// }

// // env 
// extern char **environ;

// void print_all_env(void) {
//     for (char **env = environ; *env != 0; env++) {
//         char *thisEnv = *env;
//         printf("%s\n", thisEnv);    
//     }
// }

// // CTR+Z
// void ctrZ_pid(uint32_t pid) {
//     kill(pid, SIGTSTP);
// }


// // fg
// void fg_pid(uint32_t pid) {
//     kill(pid, SIGCONT);
// }

// заменяет ${КЛЮЧ} на значение и free значение
