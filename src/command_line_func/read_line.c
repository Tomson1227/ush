#include "ush.h"

static void split_line(t_main *interface, char *line)
{
    char *clean_line = mx_del_extra_spaces(line);
    interface->line_arg.number = mx_count_words(clean_line, ' ');
    interface->line_arg.value = mx_strsplit(clean_line, ' ');
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

static int getch(t_line *line)
{
    int c = 0;

    tcgetattr(0, &line->oterm);
    memcpy(&line->term, &line->oterm, sizeof(line->term));
    line->term.c_lflag &= ~(ICANON | ECHO);
    line->term.c_cc[VMIN] = 1;
    line->term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &line->term);
    c = getchar();
    tcsetattr(0, TCSANOW, &line->oterm);
    return c;
}

static int kbhit(t_line *line)
{
    int c = 0;

    tcgetattr(0, &line->oterm);
    memcpy(&line->term, &line->oterm, sizeof(line->term));
    line->term.c_lflag &= ~(ICANON | ECHO);
    line->term.c_cc[VMIN] = 0;
    line->term.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &line->term);
    c = getchar();
    tcsetattr(0, TCSANOW, &line->oterm);
    if (c != -1) ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}

static int kbesc(t_line *line)
{
    int c;

    if (!kbhit(line)) return KEY_ESCAPE;
    c = getch(line);
    if (c == '[') {
        switch (getch(line)) {
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
    if (c == 0) while (kbhit(line)) getch(line);
    return c;
}

static int kbget(t_line *line)
{
    int c;

    c = getch(line);
    return (c == KEY_ESCAPE) ? kbesc(line) : c;
}

void read_line(t_main *interface)
{
    // printf("Debug P1\n");
    t_line *line = (t_line *) calloc(1, sizeof(t_line));
    // printf("Debug P2\n");
    init_line_struct(line);
    line->prompt = interface->prompt;
    // printf("Debug P3\n");
    new_command(&interface->command);
    // printf("Debug P4\n");
    reset_line(line);
    // printf("Debug P5\n");
    
    while (1) {
        line->symbol = kbget(line);
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
    printf("Debug 1\n");
     printf("&interface->command pos = %p\n", (void *) &interface->command);
    printf("interface->command pos = %p\n", (void *) interface->command);
    dup_command(interface->command, line->line);
    printf("Debug 2\n");
    split_line(interface, line->line);    
    printf("Debug 3\n");
    clear_line_struct(&line);
}
