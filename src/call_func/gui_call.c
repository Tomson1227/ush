#include "ush.h"

// static int get_cursore_position(t_line *line)
// {
//     char buf;
//     char numb[10];
//     int index = 0;

//     GET_CURSOR;

//     read(STDIN_FILENO, &buf, 1);

//     while(buf != ';' && buf != '\0')    
//         read(STDIN_FILENO, &buf, 1);
    
//     while(buf != 'R' && buf != '\0') {
//         numb[index++] = buf;
//         numb[index] = '\0';
//         read(STDIN_FILENO, &buf, 1);            
//     }

//     return mx_atoi(numb);
// }

void reset_cursore_position(t_line *line)
{
    RESTORE_CURSOR_POS;

    for(int index = 0; index < line->position; ++index) {
        if(line->line[index] == '\n')
            CURSOR_NEW_LINE;
        else    
            CURSUR_RIGHT;
    }
}

void reset_line(t_line *line)
{    
    RESTORE_CURSOR_POS;
    ERASE_DOWN;
    mx_printstr(line->line);
    reset_cursore_position(line);
}

static void add_char(t_line *line)
{
    if(line->position == mx_strlen(line->line)) {
        line->line[line->position++] = line->get_char;
        line->line[line->position] = '\0';
    }
    else {  
        for(int i = mx_strlen(line->line); i >= line->position; --i) {
            line->line[i + 1] = line->line[i];
            
            if(i == 0)
                break;
        }
     
        line->line[line->position++] = line->get_char;
    }

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

    reset_line(line);  
}

static void remove_char_right(t_line *line)
{
    if(line->position == mx_strlen(line->line))
        return;    
    
    for(size_t index = line->position; line->line[index]; ++index) {
        line->line[index] = line->line[index + 1];
    }

    reset_line(line);  
}

static void key_up(t_line *line, t_ush *ush) 
{
    if((void *) line->last_command->next_command) {
        if(line->last_command == ush->command_list)
            ush->command_list->command = mx_strdup(line->line);
        line->last_command = line->last_command->next_command;
        mx_strcpy(line->line, line->last_command->command);
        line->position = mx_strlen(line->line);
        reset_line(line);
    }
}

static void key_down(t_line *line, t_ush *ush) 
{ 
    if((void *) line->last_command->prev_command) {
        line->last_command = line->last_command->prev_command;
        mx_strcpy(line->line, line->last_command->command);
        line->position = mx_strlen(line->line);
        reset_line(line);
    }
}

static void key_left(t_line *line) 
{
    if(line->position > 0) {
        line->position--;
        reset_cursore_position(line);
    }   
}

static void key_right(t_line *line) 
{
    if(line->position < mx_strlen(line->line)) {
        line->position++;
        reset_cursore_position(line);
    }
}

static void key_home(t_line *line)
{
    line->position = 0;
    reset_cursore_position(line);
}

static void key_end(t_line *line)
{
    line->position = mx_strlen(line->line);
    reset_cursore_position(line);
}

static uint8_t escape_sequenses(t_line *line,t_ush *ush)
{
    read(STDIN_FILENO, &line->get_char, 1);

    if(line->get_char != '[')
        return 0;

    read(STDIN_FILENO, &line->get_char, 1);

    switch (line->get_char) {
        case 'A':   //KEY_UP
            key_up(line, ush);
            break;
        case 'B':   //KEY_DOWN
            key_down(line, ush); 
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
            read(STDIN_FILENO, &line->get_char, 1);
            switch(line->get_char) {
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
                default:
                    return 1;
            }
            break;
        case '2':
            read(STDIN_FILENO, &line->get_char, 1);
            switch(line->get_char) {
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
                default:
                    return 1;
            }
            break;
        case '3':
            read(STDIN_FILENO, &line->get_char, 1);
            switch(line->get_char) {
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
                default:
                    return 1;
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
            return 1;
    }

    return 1;
}

static uint8_t ctrl_sequenses(t_line *line)
{
    read(STDIN_FILENO, &line->get_char, 1);
    
    if(line->get_char == '^') {
        
        return 1;
    }

    return 0;
}

static uint8_t process_key_press(t_line *line, t_ush *ush)
{
    read(STDIN_FILENO, &line->get_char, 1);

    //general character
    if(line->get_char == KEY_ESCAPE) {
        escape_sequenses(line, ush);
    }
    else if(line->get_char == KEY_CTRL_SEQ) {
        ctrl_sequenses(line);
    }
    else if(line->get_char == KEY_BACKSPASE) {
        remove_char_left(line);
    }
    else if(line->get_char == KEY_TAB) {
        if(line->line[0] && line->line[0] != ' ')  {
            line->tab_func->tab_press = true;
            autocomplete(line);
        }
        else {
            line->get_char = ' ';
            add_char(line);
            add_char(line);
            add_char(line);
            add_char(line);
        }
    }
    else if(line->get_char == KEY_NEW_LINE) {
        line->position = mx_strlen(line->line);
        reset_cursore_position(line);
        ERASE_DOWN;
        
        return 0;
    }
    else {
        add_char(line);
        line->tab_func->tab_press = false;
    }

    return 1;
}

static void insert_str(char *src, char *sub)
{
    int sub_length = mx_strlen(sub);
    int src_length = mx_strlen(src);

    for(int index = 0; index <= src_length; ++index)
        src[index + sub_length] = src[index];

    for(int index = 0; sub[index]; ++index)
        src[index] = sub[index];
}

static void add_char_pos(char *str, char c, int pos)
{
    for(int index = mx_strlen(str); pos <= index; --index) {
        str[index + 1] = str[index];
    }

    str[pos] = c;
}

static void add_line(t_ush *ush, t_line *line)
{
    char *copy_line = mx_strdup(line->line);
    line->line[0] = '\0';
    line->position = 0;

    mx_printstr("> ");
    SAVE_CURSOR_POS;
    while(process_key_press(line, ush));

    add_char_pos(line->line, '\n', 0);        
    insert_str(line->line, copy_line);
    mx_strdel(&copy_line);
}

void get_user_input(t_line *line, t_ush *ush)
{
    tcsetattr(0, TCSANOW, &ush->oterm); 

    while(process_key_press(line, ush));
    
    mx_printchar('\n');

    while(mx_count_chars(line->line, '\"') % 2) {
       add_line(ush, line);
    }

    tcsetattr(0, TCSANOW, &ush->term);
}

/* Debug */
void print_arg(char **args)
{
    for(uint16_t index = 0; args[index]; ++index) {
        mx_printstr(args[index]);
        mx_printchar('$');
    }
    
    mx_printchar('\n');
}

void gui_call(t_ush *ush)
{
    new_command_list(&ush->command_list);

    t_line *line = (t_line *) calloc(1, sizeof(t_line));
    init_line_struct(line, ush);

    mx_printstr(ush->prompt);
    SAVE_CURSOR_POS;
    reset_line(line);
    get_user_input(line, ush);
    validate_args(ush, line->line);

    // print_arg(ush);

    dup_command(ush->command_list, line->line);
    del_line_struct(&line);
}
