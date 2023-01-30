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

int insert(t_list l, ElementType x, t_position p)
{
    ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
    if (tmp_cell == NULL)
        return -1;
    tmp_cell->element = x;
    tmp_cell->next = p->next;
    p->next = tmp_cell;
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

// swap index_th element and (index+1)_th element of pos
int swap_list(t_list list, int index, t_position pos)
{
    for (int i = 0; i < index && pos != NULL; i++)
        pos = pos->next;
    if (pos->next == NULL || pos->next->next == NULL)
        return -1;
    ptr_to_node next_node = pos->next;
    ptr_to_node next_next_node = next_node->next;
    next_node->next = next_next_node->next;
    next_next_node->next = next_node;
    pos->next = next_next_node;
}

int main()
{
    ptr_to_node l = (ptr_to_node)malloc(sizeof(struct t_node));
    l->element = 0;
    l->next = NULL;
    ptr_to_node p = l;
    for (int i = 0; i < 10; i++)
    {
        insert(l, i + 1, p);
        p = p->next;
    }
    printf("\nlist before swap:");
    print(l);
    p = l;
    swap_list(l, 8, p);
    printf("\nlist after swap:");
    print(l);
    printf("\n");
}
