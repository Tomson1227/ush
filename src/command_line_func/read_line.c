#include "ush.h"

static struct termios term, oterm;

static int getch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);

typedef struct s_line {
    char *prompt;
    char *line;
    size_t size;
    size_t position;
    int symbol;
}              t_line;

static void split_line(t_main *interface, char *line)
{
    char *clean_line = mx_del_extra_spaces(line);
    interface->line_arg.number = mx_count_words(clean_line, ' ');
    interface->line_arg.value = mx_strsplit(clean_line, ' ');
}

static inline void init_line_struct(t_line *line)
{
    size_t prompt_size = snprintf(NULL, 0, "%s%sU$H>%s ", BOLD, GREEN, DEFAULT_COLLOR);
    line->prompt = (char *) calloc(prompt_size, sizeof(char));
    sprintf(line->prompt, "%s%sU$H>%s ", BOLD, GREEN, DEFAULT_COLLOR);

    line->line = (char *) calloc(BUFSIZE, sizeof(char));
    line->size = 0;
    line->position = 0;
    line->symbol = '\0';
}

static void reset_line(t_line *line)
{    
    write(STDOUT_FILENO, "\r", 1);
    for(uint32_t i = 0; i < line->size + mx_strlen(line->prompt); ++i)
        write(STDOUT_FILENO, &" ", 1);
    write(STDOUT_FILENO, "\r", 1);
    write(STDOUT_FILENO, line->prompt, mx_strlen(line->prompt));
    write(STDOUT_FILENO, (void *) line->line, line->size);
    if(line->size != line->position)
        cursorbackward((int) (line->size - line->position));
}

static void add_char(t_line *line)
{
    if(line->position == line->size) {
        line->line[line->position++] = line->symbol;
        line->line[line->position] = '\0';
    }
    else {
        for(size_t i = line->size; i >= line->position; --i) {
            line->line[i + 1] = line->line[i];
            
            if(i == 0)
                break;
        }
     
        line->line[line->position++] = line->symbol;
    }

    line->size++;
    reset_line(line);
}

static void remove_char(t_line *line)
{
    line->position--;

    for(size_t index = line->position; line->line[index]; ++index) {
        line->line[index] = line->line[index + 1];
    }
    
    line->size--;
    reset_line(line);  
} 

static int getch(void)
{
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    return c;
}

static int kbhit(void)
{
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    if (c != -1) ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}

static int kbesc(void)
{
    int c;

    if (!kbhit()) return KEY_ESCAPE;
    c = getch();
    if (c == '[') {
        switch (getch()) {
            case 'A':
                c = KEY_UP;
                break;
            case 'B':
                c = KEY_DOWN;
                break;
            case 'C':
                c = KEY_RIGHT;
                break;
            case 'D':
                c = KEY_LEFT;
                break;
            default:
                c = 0;
                break;
        }
    } else {
        c = 0;
    }
    if (c == 0) while (kbhit()) getch();
    return c;
}

static int kbget(void)
{
    int c;

    c = getch();
    return (c == KEY_ESCAPE) ? kbesc() : c;
}

void read_line(t_main *interface)
{
    t_line *line = (t_line *) calloc(1, sizeof(t_line));
    init_line_struct(line);
    // write(STDOUT_FILENO, line->prompt, mx_strlen(line->prompt));
    reset_line(line);
    
    while (1) {
        line->symbol = kbget();
        if (line->symbol == KEY_ENTER || line->symbol == KEY_ESCAPE || line->symbol == KEY_UP || line->symbol == KEY_DOWN) {
            break;
        } 
        else if (line->symbol == KEY_RIGHT) {
            if(line->position < line->size) {
                cursorforward(1);
                line->position++;
            }
        } 
        else if (line->symbol == KEY_LEFT) {
            if(line->position > 0) {
                cursorbackward(1);
                line->position--;
            }     
        } 
        else if(line->symbol == KEY_DEL) {
            if(line->position > 0) {
                remove_char(line);   
            }                
        }
        else {
            if(line->size < BUFSIZE)
                add_char(line);
            else
                break;
        }
    }

    write(STDOUT_FILENO, &"\n", 1);
    split_line(interface, line->line);    
}
