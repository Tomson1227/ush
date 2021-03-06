#ifndef KEYDEF_H
#define KEYDEF_H

#define BUFSIZE     1024

#define READ_OPT(reg, flag) !!(reg & (flag))

#define KEY_BACKSPASE   0x7F
#define KEY_NEW_LINE    0x0A
#define KEY_ESCAPE      0x1B
#define KEY_CTRL_SEQ    0x5E
#define KEY_TAB         0x09

#define ESC                 "\033"

/*    C1 set element ANSI escape sequences    */
#define CSI     ESC "["     //Controll Sequence Initiator
#define SS2     ESC "N"     //Single Shift Two
#define SS3     ESC "O"     //Single Shift Three
#define DCS     ESC "P"     //Device Control String
#define ST      ESC "\\"    //String Terminator
#define OSC     ESC "]"     //Operating System Command
#define SOS     ESC "X"     //Start Of String
#define PM      ESC "^"     //Privacy Message
#define APC     ESC "_"     //Aplication Program Command

/*    SGR(Select Grafical Rendition) parametrs    */
#define SGR_END             "m"

#define BOLD                CSI "1"  SGR_END
#define FAINT               CSI "2"  SGR_END
#define ITALIC              CSI "3"  SGR_END
#define UNDERLINE           CSI "4"  SGR_END
#define INVERT_COLOR        CSI "7"  SGR_END
#define TEXT_HIDE           CSI "8"  SGR_END
#define DEFAULT_FONT        CSI "10" SGR_END

/* FG - ForeGround */
#define FG_COLOR_RED        CSI "31" SGR_END
#define FG_COLOR_GREEN      CSI "32" SGR_END
#define FG_COLOR_ORANGE     CSI "33" SGR_END
#define FG_COLOR_BLUE       CSI "34" SGR_END
#define FG_COLOR_MAGENTA    CSI "35" SGR_END
#define FG_COLOR_CYAN       CSI "36" SGR_END
#define FG_COLOR_LIGHT_GRAY CSI "37" SGR_END
#define FG_COLOR_RESET      CSI "39" SGR_END

/* BG - BackGround */
#define BG_COLOR_BLACK      CSI "40" SGR_END
#define BG_COLOR_RED        CSI "41" SGR_END
#define BG_COLOR_GREEN      CSI "42" SGR_END
#define BG_COLOR_ORANGE     CSI "43" SGR_END
#define BG_COLOR_BLUE       CSI "44" SGR_END
#define BG_COLOR_MAGENTA    CSI "45" SGR_END
#define BG_COLOR_CYAN       CSI "46" SGR_END
#define BG_COLOR_LIGHT_GRAY CSI "47" SGR_END
#define BG_COLOR_RESET      CSI "49" SGR_END

#define RESET_ALL           CSI "0"  SGR_END

/* r,g,b can be 0 - 255 each */
#define SET_FG_RGB(r,g,b)   mx_printstr(CSI "38:2:"), \
                            mx_printint(r), \
                            mx_printchar(':'), \
                            mx_printint(g), \
                            mx_printchar(':'), \
                            mx_printint(b), \
                            mx_printchar('m')
/* r,g,b can be 0 - 255 each */
#define SET_BG_RGB(r,g,b)   mx_printstr(CSI "48:2:"), \
                            mx_printint(r), \
                            mx_printchar(':'), \
                            mx_printint(g), \
                            mx_printchar(':'), \
                            mx_printint(b), \
                            mx_printchar('m')

/* n can be 0 - 255 */
#define SET_FG_NUM(n)       mx_printstr(CSI "38:5:"), \
                            mx_printint(r), \
                            mx_printchar('m')
/* n can be 0 - 255 */
#define SET_BG_NUM(n)       mx_printstr(CSI "48:5:"), \
                            mx_printint(r), \
                            mx_printchar('m')

#define MV_CURSOR(row,column)       mx_printstr(CSI), \
                                    mx_printint(row), \
                                    mx_printchar(';'), \
                                    mx_printint(column), \
                                    mx_printchar('f')

/*    Terminal input sequences    */
#define INSERT_KEY          CSI "2~"
#define DELETE_KEY          CSI "3~"
#define PG_UP               CSI "5~"
#define PG_DOWN             CSI "6~"
#define F1_KEY              CSI "1P"
#define F2_KEY              CSI "1Q"
#define F3_KEY              CSI "1R"
#define F4_KEY              CSI "1S"

#define SAVE_CURSOR_POS     mx_printstr(CSI "s")
#define RESTORE_CURSOR_POS  mx_printstr(CSI "u")
#define HIDE_CURSOR         mx_printstr(CSI "?25l")
#define SHOW_CURSOR         mx_printstr(CSI "?25h")

#define ERASE_RIGHT         mx_printstr(CSI "0K")
#define ERASE_LEFT          mx_printstr(CSI "1K")
#define ERASE_LINE          mx_printstr(CSI "2K")

#define ERASE_DOWN          mx_printstr(CSI "0J")
#define ERASE_UP            mx_printstr(CSI "1J")
#define ERASE_ALL           mx_printstr(CSI "2J")

#define SCROLL_UP           mx_printstr(CSI "1S")
#define SCROLL_DOWN         mx_printstr(CSI "1T")

#define CURSUR_RIGHT        mx_printstr(CSI "1C")
#define CURSUR_LEFT         mx_printstr(CSI "1D")
#define CURSOR_UP           mx_printstr(CSI "1A")
#define CURSOR_DOWN         mx_printstr(CSI "1B")

#define CURSOR_NEW_LINE     mx_printstr(CSI "1E")
#define CURSOR_PREV_LINE    mx_printstr(CSI "1F")

/* x = [0 ... 9]*/
#define CURSOR_N_UP(x)      mx_printstr(CSI),   \
                            mx_printint(x),     \
                            mx_printchar('A')
/* x = [0 ... 9]*/
#define CURSOR_N_DOWN(x)    mx_printstr(CSI),   \
                            mx_printint(x),     \
                            mx_printchar('B')
/* x = [0 ... 9]*/
#define CURSOR_N_RIGHT(x)   mx_printstr(CSI),   \
                            mx_printint(x),     \
                            mx_printchar('C')
/* x = [0 ... 9]*/
#define CURSOR_N_LEFT(x)    mx_printstr(CSI),   \
                            mx_printint(x),     \
                            mx_printchar('D')

#define GET_CURSOR          mx_printstr(CSI "6n")

#endif /* KEYDEF_H */
