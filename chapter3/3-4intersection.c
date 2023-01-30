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

int intersection_list(t_list list1, t_list list2, t_list intersect_list)
{
    ptr_to_node pos1 = list1->next;
    ptr_to_node pos2 = list2->next;
    ptr_to_node pos_intersect = intersect_list;
    while (pos1 != NULL && pos2 != NULL)
    {
        int elem1 = pos1->element;
        int elem2 = pos2->element;
        if (elem1 < elem2)
            pos1 = pos1->next;
        else if (elem1 > elem2)
            pos2 = pos2->next;
        else
        {
            ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
            tmp_cell->element = elem1;
            tmp_cell->next = pos_intersect->next;
            pos_intersect->next = tmp_cell;
            pos_intersect = tmp_cell;
            pos1 = pos1->next;
            pos2 = pos2->next;
        }
    }
}

int main()
{
    const int len1 = 6;
    const int a1[6] = {3, 7, 11, 15, 20, 23};
    const int len2 = 8;
    const int a2[8] = {2, 4, 7, 10, 15, 20, 22, 25};

    ptr_to_node list1 = (ptr_to_node)malloc(sizeof(struct t_node));
    list1->element = 0;
    list1->next = NULL;
    ptr_to_node p = list1;
    for (int i = 0; i < len1; i++)
    {
        insert(list1, a1[i], p);
        p = p->next;
    }
    ptr_to_node list2 = (ptr_to_node)malloc(sizeof(struct t_node));
    list2->element = 0;
    list2->next = NULL;
    p = list2;
    for (int i = 0; i < len2; i++)
    {
        insert(list2, a2[i], p);
        p = p->next;
    }
    printf("\nlist1: ");
    print(list1);
    printf("\nlist2: ");
    print(list2);
    ptr_to_node result = (ptr_to_node)malloc(sizeof(struct t_node));
    result->element = 0;
    result->next = NULL;
    intersection_list(list1, list2, result);
    printf("\nintersection_list: ");
    print(result);
    printf("\n");
}
