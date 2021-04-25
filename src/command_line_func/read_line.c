#include "ush.h"

static void line_close(t_line **line, t_main *interface)
{
    dup_command(interface->command, (*line)->line);
    char *clean_line = mx_del_extra_spaces((*line)->line);
    interface->line_arg.number = mx_count_words(clean_line, ' ');
    interface->line_arg.value = mx_strsplit(clean_line, ' ');  
    clear_line_struct(line);
}

static void reset_line(t_line *line)
{    
    mx_printstr(RESTORE_CURSOR_POS);
    ERASE_RIGHT;
    write(STDOUT_FILENO, (void *) line->line, strlen(line->line));
    // mx_printstr("line position = ");
    // mx_printint(line->position);
    // mx_printchar('\n');
    mx_printstr(RESTORE_CURSOR_POS);
    if(line->position)
        CURSOR_N_RIGHT((uint32_t)(line->position));
}

static void get_key_press(t_line *line)
{
    int temp;

    for(uint8_t index = 0; index < 10; ++index) 
        line->key_press[index] = '\0';
        
    tcsetattr(0, TCSANOW, &line->oterm1);
    read(STDIN_FILENO,line->key_press, 10);

    // for(uint8_t index = 0; line->key_press[index]; ++index) {
    //     if(line->key_press[index] == KEY_ESCAPE)
    //         mx_printstr("ESC");
    //     else
    //         mx_printint(line->key_press[index]);
    //     mx_printchar(' ');
    // }
    // mx_printchar('\n');

    tcsetattr(0, TCSANOW, &line->term);
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

static void remove_char_left(t_line *line)
{
    if(line->position == 0)
        return;

    line->position--;
    
    for(size_t index = line->position; line->line[index]; ++index) {
        line->line[index] = line->line[index + 1];
    }

    line->size--;
    reset_line(line);  
}

static void remove_char_right(t_line *line)
{
    if(line->position == line->size)
        return;    
    
    for(size_t index = line->position; line->line[index]; ++index) {
        line->line[index] = line->line[index + 1];
    }

    line->size--;
    reset_line(line);  
}

static void key_up(t_line *line, t_main *interface) 
{
    if((void *) line->last_commant->next_command) {
        if(line->last_commant == interface->command)
            interface->command->command = mx_strdup(line->line);
        line->last_commant = line->last_commant->next_command;
        mx_strcpy(line->line, line->last_commant->command);
        line->position = line->size = mx_strlen(line->line);
        reset_line(line);
    }
}

static void key_down(t_line *line, t_main *interface) 
{ 
    if((void *) line->last_commant->prev_command) {
        line->last_commant = line->last_commant->prev_command;
        mx_strcpy(line->line, line->last_commant->command);
        line->position = line->size = mx_strlen(line->line);
        reset_line(line);
    }
}

static void key_left(t_line *line) 
{
    if(line->position > 0) {
        CURSUR_LEFT;
        line->position--;
    }   
}

static void key_right(t_line *line) 
{
    if(line->position < line->size) {
        CURSUR_RIGHT;
        line->position++;
    }
}

static void key_home(t_line *line)
{
    line->position = 0;
    reset_line(line);
}

static void key_end(t_line *line)
{
    line->position = line->size;
    reset_line(line);
}

static uint8_t escape_sequenses(t_line *line,t_main *interface)
{
    if(line->key_press[1] != '[')
        return 0;

    switch (line->key_press[2]) {
        break;
    case 'A':   //KEY_UP
        key_up(line, interface);
        break;
    case 'B':   //KEY_DOWN
        key_down(line, interface); 
        break;
    case 'C':   //KEY_RIGHT
        key_right(line);
        break;
    case 'D':   //KEY_LEFT
        key_left(line);
        break;
    case 'F':   //KEY_END
        key_end(line);
        break;
    case 'H':   //KEY_HOME
        key_home(line);
        break;
    case '1':
        switch(line->key_press[3]) {
        case '~':   //HOME_KEY
            key_home(line);
            break;
        case '0':   //F0_KEY
            break;
        case '1':   //F1_KEY
            break;
        case '2':   //F2_KEY
            break;  
        case '3':   //F3_KEY
            break;
        case '4':   //F4_KEY
            break;
        case '5':   //F5_KEY
            break;
        case '7':   //F6_KEY
            break;
        case '8':   //F7_KEY
            break;
        case '9':   //F8_KEY
            break;
        case 'P':   //F1_KEY
            break;
        case 'Q':   //F2_KEY
            break;
        case 'R':   //F3_KEY
            break;
        case 'S':   //F4_KEY
            break;
        }
        break;
    case '2':
        switch(line->key_press[3]) {
        case '~':   //INSERT_KEY
            break;
        case '0':   //F9_KEY
            break;
        case '1':   //F10_KEY
            break;
        case '3':   //F11_KEY
            break;
        case '4':   //F12_KEY
            break;
        case '5':   //F13_KEY
            break;
        case '6':   //F14_KEY
            break;
        case '8':   //F15_KEY
            break;
        case '9':   //F16_KEY
            break;
        }
        break;
    case '3':
        switch(line->key_press[3]) {
        case '~':   // KEY DELATE
            remove_char_right(line);
            break;
        case '1':   //F17_KEY
            break;
        case '2':   //F18_KEY
            break;
        case '3':   //F19_KEY
            break;
        case '4':   //F20_KEY
            break;
        }
        break;
    case '4':   //END_KEY
        key_end(line);
        break;
    case '5':   //PG_UP
        break;
    case '6':   //PG_DOWN
        break;
    case '7':   //HOME_KEY
        key_home(line);
        break;
    case '8':   //END_KEY 
        key_end(line);
        break;
    case '\n':  //EOL
        return 0;
    default:
        return 0;
    }
    return 1;
}

static uint8_t ctrl_sequenses(t_line *line,t_main *interface)
{
    if(line->key_press[0] == '^') {
        
        return 1;
    }

    return 0;
}

static uint8_t process_key_press(t_line *line, t_main *interface)
{
    get_key_press(line);

    //general character
    if(line->key_press[0] == KEY_ESCAPE) {
        return escape_sequenses(line, interface);
    }
    else if(line->key_press[0] == KEY_CTRL_SEQ) {
        return ctrl_sequenses(line, interface);
    }
    else if(line->key_press[0] == KEY_BACKSPASE) {
        remove_char_left(line);
        return 1;
    }
    else if(line->key_press[0] == KEY_NEW_LINE) {
        return 0;
    }
    else {
        line->symbol = line->key_press[0];
        add_char(line);
        return 1;
    }
}

void read_line(t_main *interface)
{
    new_command(&interface->command);
    t_line *line = (t_line *) calloc(1, sizeof(t_line));
    init_line_struct(line, interface);
    write(STDOUT_FILENO, interface->prompt, mx_strlen(interface->prompt));
    mx_printstr(SAVE_CURSOR_POS);
    reset_line(line);
    
    while(process_key_press(line, interface));

    write(STDOUT_FILENO, &"\n", 1);
    line_close(&line,interface);
}
