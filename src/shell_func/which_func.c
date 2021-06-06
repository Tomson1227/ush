#include "ush.h"

/*
-a List all instances of executables found (instead of just the first one of each).
-s No output, just return 0 if all of the executables are found, or 1 if some were not found.
*/
void which_func(t_ush *ush, t_process *process)
{
    uint32_t options = get_opt(process->args, "as");
    uint8_t opt_a = READ_OPT(options, 1);
    uint8_t opt_s = READ_OPT(options, 2);

    for(uint8_t index = 1; process->args[index]; ++index) {    
        if(process->args[index][0] == '-')
            continue;
        
        bool status = 1;

        for (int i = 0; status && ush->built_in[i]; i++) {
            if (!strcmp(ush->built_in[i], process->args[index])) {
                char *message = mx_strjoin(ush->built_in[i], ": ush built-in command");
                
                if(opt_a || !opt_s) {
                    printf("%s\n", message);
                    mx_strdel(&message);
                }
                
                status = 0;
            }
        }

        for(uint8_t i = 0; (status || opt_a) && ush->bin_dirs[i]; ++i) {
            char *temp = mx_strjoin(ush->bin_dirs[i], "/");
            char *path = mx_strjoin(temp, process->args[index]);
            mx_strdel(&temp);

            if(!access(path, F_OK) && (opt_a || !opt_s)) {
                printf("%s\n", path);
                status = 0;
            }

            mx_strdel(&path);
        }

        if(status && !opt_s)
            printf("%s not found\n", process->args[index]);

        process->status |= status;
    }
}
