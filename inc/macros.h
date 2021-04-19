#ifndef MACROS_H
#define MACROS_H

#define BOLD                "\033[1m"

//Color set
#define BLACK               "\033[30m"
#define RED                 "\033[31m"
#define GREEN               "\033[32m"
#define ORANGE              "\033[33m"
#define BLUE                "\033[34m"
#define MAGENTA             "\033[35m"
#define CYAN                "\033[36m"
#define LIGHT_GRAY          "\033[37m"
#define DEFAULT_COLLOR      "\033[0m"

#define BLACK_BG            "\033[40m"
#define	RED_BG              "\033[41m"
#define GREEN_BG            "\033[42m"
#define ORANGE_BG           "\033[43m"
#define BLUE_BG             "\033[44m"
#define MAGENTA_BG          "\033[45m"
#define CYAN_BG             "\033[46m"
#define LIGHT_GRAY_BG       "\033[47m"
#define DEFAULT_COLLOR_BG   "\033[49m"


#define cursorforward(x) printf("\033[%dC", (x))
#define cursorbackward(x) printf("\033[%dD", (x))

#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x000a
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108
#define KEY_DEL     0x007F

#define BUFSIZE     1024

#endif /* MACROS_H */
