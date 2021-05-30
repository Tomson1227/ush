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
char *replacement(char *src, int from_id, int to_id, char *value) 
{
    int env_len = value == NULL ? 0 : strlen(value);
    int new_str_len = strlen(src) - (to_id - from_id) + env_len;
    char *new_str = (char*)calloc(new_str_len + 1, sizeof(char));
    strncpy(new_str, src, from_id);

    if(value != NULL) {
        strncpy(&new_str[from_id], value, env_len);
        strncpy(&new_str[from_id + env_len], &src[to_id], strlen(src) - to_id);
    } 
    else
        strncpy(&new_str[from_id + env_len - 1], &src[to_id], strlen(src) - to_id);

    free(src);
    return new_str;
}

    // ищет ${КЛЮЧ} в str
char *find_value(char *str) 
{
    for(int start = 0; str[start]; ++start) {
        if(str[start] == '$' && str[start + 1] == '{') {
            int finish = start;

            while(str[finish] && str[finish++] != '}');

            char env_key[finish - start - 2];
            env_key[finish - start - 3] = '\0';
            strncpy(env_key, &str[start + 2], finish - start - 3);
            char *value = get_env_value(env_key);

            str = replacement(str, start, finish, value);
            free(value);

            // if(value == NULL) { // not find
            //     str = replacement(str, start, finish, value);
            // } else {
            //     str = replacement(str, start, finish, value);
            //     free(value);
            // }

            start = 0;
        }
    }
    return str;
}

// int main() {
//     char *a1 = "${SHLVL} ${HOME} ${LOGNAME} ${USR} ${TERM}";
//     char *a = (char *)malloc(strlen(a1) * sizeof(char));

//     strcpy(a, a1);
//         printf("---%s---\n\n\n", a);
//     char *newstr = find_value(a);
//         printf("%s\n", newstr);
//         //${SHLVL} ${HOME} ${LOGNAME} ${USR} ${TERM}
// }
