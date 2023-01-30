/*
note:
1. if we need to delete one element, bidirectional list is needed
*/

#include<stdlib.h>
#include<stdio.h>

struct t_node;
typedef struct t_node *ptr_to_node;
typedef ptr_to_node t_list;
typedef ptr_to_node t_position;

struct t_node
{
    int id;
    t_position next;
    t_position prev;
};

int create_loop_bidirection_list(t_list lst, int number)
{
    if (number < 1)
        return -1;
    t_position pos = lst;
    pos->id = 1;
    pos->next = lst;
    // pos->prev = lst;
    for (int i = 1; i < number; i++)
    {
        ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
        tmp_cell->id = i + 1;
        tmp_cell->next = pos->next;
        tmp_cell->prev = pos;
        pos->next = tmp_cell;
        pos = tmp_cell;
    }
    lst->prev = pos;
    return 0;
}

int print_list(t_list lst)
{
    ptr_to_node pos = lst;
    printf("%d ", pos->id);
    pos = pos->next;
    while (pos != lst)
    {
        printf("%d ", pos->id);
        pos = pos->next;
    }
    printf("\n");
    return 0;
}

int free_list(t_list lst)
{
    if (lst == NULL)
        return -1;
    ptr_to_node cur_p = lst->next;
    ptr_to_node del_p = cur_p;
    while (cur_p != lst)
    {
        cur_p = cur_p->next;
        free(del_p);
        del_p = cur_p;
    }
    free(cur_p);
    return 0;
}

int josephus(int m, int n)
{
    t_list lst = (t_list)malloc(sizeof(struct t_node));
    create_loop_bidirection_list(lst, n);
    t_position cur_p = lst;
    t_position del_p;
    t_position prev_p;
    t_position next_p;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cur_p = cur_p->next;
        }
        prev_p = cur_p->prev;
        next_p = cur_p->next;
        prev_p->next = next_p;
        next_p->prev = prev_p;
        del_p = cur_p;
        cur_p = cur_p->next;
        printf("%d ", del_p->id);
        free(del_p);
    }
}

int main()
{
    int number = 5;
    int m = 1;
    josephus(m, number);
}