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

int print(t_list l)
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

int find(t_list lst, ElementType x)
{
    if (lst == NULL)
        return -1;
    t_position pos = lst->next;
    int id = 0;
    while (pos != NULL)
    {
        if (pos->element == x)
            return id;
        id++;
        pos = pos->next;
    }
    return -1;
}

int id_recursion = 0;

int find_recursion(t_list lst, ElementType x)
{
    if (lst == NULL || lst->next == NULL)
        return -1;
    ptr_to_node pos = lst->next;
    if (pos->element == x)
        return id_recursion;
    id_recursion++;
    return find_recursion(pos, x);
}

int main()
{
    int number = 180000;
    ptr_to_node lst = (ptr_to_node)malloc(sizeof(struct t_node));
    init_list(lst);
    create_list(lst, number);
    int id = find(lst, number);
    printf("id: %d \n", id);

    int id_r = find_recursion(lst, number);
    printf("id: %d \n", id_r);
    free_list(lst);
}
