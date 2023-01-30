/*
note:
1. note the difference between 'pointer variable' and 'pointer returned by malloc' 
*/

#include <stdlib.h>
#include <stdio.h>
#define ElementType int

struct t_node;
typedef struct t_node *ptr_to_node;
typedef ptr_to_node t_list;
typedef ptr_to_node t_position;

struct t_node
{
    ElementType element;
    t_position next;
};

int init_list(t_list lst)
{
    lst->element = (ElementType)0;
    lst->next = NULL;
    return 0;
}

int print_list(t_list l)
{
    if (l->next == NULL)
        return 0; // empty list
    ptr_to_node p = l->next;
    while (p != NULL)
    {
        printf("%d ", p->element);
        p = p->next;
    }
    return 0;
}

int create_list(t_list lst, int number)
{
    if (number < 1 || lst == NULL)
        return -1;
    t_position pos = lst;
    for (int i = 0; i < number; i++)
    {
        t_position tmp_cell = (t_position)malloc(sizeof(struct t_node));
        tmp_cell->element = (ElementType)(i + 1);
        tmp_cell->next = pos->next;
        pos->next = tmp_cell;
        pos = pos->next;
    }
}

int free_list(t_list lst)
{
    ptr_to_node del_p;
    while (lst != NULL)
    {
        del_p = lst;
        lst = lst->next;
        free(del_p);
    }
    free(lst);
    return 0;
}

int reverse_list(t_list source, t_list dest)
{
    if (source == NULL || dest == NULL)
        return -1;
    t_position pos_source = source->next;
    t_position pos_dest = dest;
    while (pos_source != NULL)
    {
        t_position tmp_cell = (t_position)malloc(sizeof(struct t_node));
        tmp_cell->element = pos_source->element;
        tmp_cell->next = pos_dest->next;
        pos_dest->next = tmp_cell;
        pos_source = pos_source->next;
    }
}

int reverse_list_inplace(t_list source, t_list dest)
{
    if (source == NULL || dest == NULL)
        return -1;
    t_position pos_source = source->next;
    t_position pos_dest = dest;
    t_position pos_tmp;
    while (pos_source != NULL)
    {
        pos_tmp = pos_source;
        pos_source = pos_source->next;
        pos_tmp->next = pos_dest->next;
        pos_dest->next = pos_tmp;
    }
}

int main()
{
    int number = 10;
    ptr_to_node lst = (ptr_to_node)malloc(sizeof(struct t_node));
    init_list(lst);
    create_list(lst, number);
    ptr_to_node dest = (ptr_to_node)malloc(sizeof(struct t_node));
    // if (reverse_list(lst, dest) != -1)
    // {
    //     print_list(dest);
    //     printf("\n");
    // }
    // free_list(lst);
    // free_list(dest);

    // constant memory usage
    if (reverse_list_inplace(lst, dest) != -1)
    {
        print_list(dest);
        printf("\n");
    }
    free_list(dest);
    free(lst);
}
