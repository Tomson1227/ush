#include "ush.h"

static t_command_list *init_command(void)
{
    t_command_list *new_node = NULL;

    if(!(new_node = (t_command_list *) calloc(1, sizeof(t_command_list))))
        strerror(errno);
    
    new_node->command = NULL;
    new_node->next_command = NULL;
    new_node->prev_command = NULL;

    return new_node;
}

void set_command(t_command_list *head, char *command)
{
    if(malloc_size(head->command)) {
        free(head->command);
    }

    head->command = command;
}

void dup_command(t_command_list *head, char *command)
{
    if(malloc_size(head->command)) {
        free(head->command);
    }

    head->command = mx_strdup(command);
}

void new_command(t_command_list **head)
{
    t_command_list *new_command = init_command();
    
    if(!(*head)) {
        (*head) = new_command;        
        return;
    }

    new_command->next_command = *head;
    (*head)->prev_command = new_command;
    (*head) = new_command;
}

void del_last_command(t_command_list **head)
{
    t_command_list *temp = *head;

	if (!head || !*head)
		return;

    while(temp->next_command)
        temp = temp->next_command;

    temp->prev_command->next_command = NULL;
    temp->prev_command = NULL;
    free(temp->command);
    temp->command = NULL;
}

void del_command_list(t_command_list **head)
{
    t_command_list *temp = *head;

    if (!head || !*head)
		return;
    
    while(temp) {
        free((*head)->command);
        (*head)->command = NULL;
        (*head)->prev_command = NULL;
        temp = (*head)->next_command;
        (*head)->next_command = NULL;
        free(*head);
        (*head) = temp;
    }
}



