#include "ush.h"

char *read_line(void)
{
  char *line = NULL;
  size_t bufsize = 0; 

  if (getline(&line, &bufsize, stdin) == -1){
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);  // We recieved an EOF
    } 
    else  {
      perror("readline");
      exit(EXIT_FAILURE);
    }
  }

  return line;
}

// #define LSH_RL_BUFSIZE 1024
// char *lsh_read_line(void)
// {
//   int bufsize = LSH_RL_BUFSIZE;q
//   int position = 0;
//   char *buffer = malloc(sizeof(char) * bufsize);
//   int c;

//   if (!buffer) {
//     fprintf(stderr, "lsh: allocation error\n");
//     exit(EXIT_FAILURE);
//   }

//   while (1) {
//     // Read a character
//     c = getchar();

//     // If we hit EOF, replace it with a null character and return.
//     if (c == EOF || c == '\n') {
//       buffer[position] = '\0';
//       return buffer;
//     } else {
//       buffer[position] = c;
//     }
//     position++;

//     // If we have exceeded the buffer, reallocate.
//     if (position >= bufsize) {
//       bufsize += LSH_RL_BUFSIZE;
//       buffer = realloc(buffer, bufsize);
//       if (!buffer) {
//         fprintf(stderr, "lsh: allocation error\n");
//         exit(EXIT_FAILURE);
//       }
//     }
//   }
// }
