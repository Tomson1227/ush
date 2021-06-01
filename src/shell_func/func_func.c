#include "ush.h"

/* 
GET:
char **argv - NULL terminated array
char *search_opt - string with optinons chars (max 32)
RETURN:
32-b registr fill with coresponding char option in the same position
*/
uint32_t get_opt(char **argv, char *seach_opt)
{
    extern int optind;
    int c, pos;
    uint32_t find_opt = 0, argc;

    for(argc = 0; argv[argc]; ++argc);

    while((c = getopt(argc, argv, seach_opt)) != -1)
        find_opt |= 1 << mx_get_char_index(seach_opt, c);

    optind = 1;
    return find_opt;
}

uint32_t count_args(char **argv)
{
    uint32_t count;

    for(count = 0; argv[count]; ++count);

    return count;
}

uint32_t start_index(char **argv)
{
    uint32_t count;

    for(count = 0; argv[count][0] == '-'; ++count);

    return count;
}
