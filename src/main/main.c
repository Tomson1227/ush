#include "ush.h"

int main(int argn, char *argv[], char **environ)
{
    t_ush *ush = NULL;
    init_ush_struct(&ush);

    if(isatty(0)) {
        while(ush->status == -1) {
            gui_call(ush);
            execute(ush);
        }
    }
    else 
        pipe_call(ush);

    del_ush_struct(&ush);
    return ush->status;
}
