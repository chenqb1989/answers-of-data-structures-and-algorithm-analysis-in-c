/*
note:
1. be careful of 'position pointers update' when delete nodes (line 143-153)
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct t_node;
typedef struct t_node *ptr_to_node;
typedef ptr_to_node t_list;
typedef ptr_to_node t_position;

struct t_node
{
    int element;
    int is_deleted;
    t_position next;
};

int append(t_list lst, int elem)
{
    if (lst == NULL)
        return -1;
    t_position pos = lst;
    while (pos->next != NULL)
        pos = pos->next;
    t_position tmp_cell = (t_position)malloc(sizeof(struct t_node));
    tmp_cell->element = elem;
    tmp_cell->is_deleted = 0;
    tmp_cell->next = pos->next;
    pos->next = tmp_cell;
    return 0;
}

int init_list(t_list lst)
{
    lst->element = 0;
    lst->is_deleted = 0;
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
        if (p->is_deleted == 1)
            printf("%dd ", p->element);
        else
            printf("%d ", p->element);
        p = p->next;
    }
    printf("\n");
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

int create_list(t_list lst, int *array, int len)
{
    if (lst == NULL || array == NULL || len <= 0)
        return -1;
    for (int i = 0; i < len; i++)
        append(lst, array[i]);
    return 0;
}

int create_array(int *array, int len)
{
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    // for (int i = 0; i < len; i++)
    //     array[i] = i;
    for (int i = 0; i < len; i++)
    {
        int r = rand() % 10;
        // int tmp = array[i];
        // array[i] = array[r];
        array[i] = r;
    }
    return 0;
}

int print_array(int *array, int len)
{
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    for (int i = 0; i < len; i++)
        printf("%d ", array[i]);
    printf("\n");
    return 0;
}

int lazy_del_duplicate_list(t_list lst)
{
    if (lst == NULL)
        return 0;
    t_position pos1 = lst->next;
    t_position pos2;
    int total = 0;
    int deleted = 0;
    while(pos1 != NULL)
    {
        total++;
        pos2 = pos1->next;
        while (pos2 != NULL)
        {
            if (pos1->element == pos2->element)
            {
                pos2->is_deleted = 1;
                deleted++;
            }
            pos2 = pos2->next;
        }
        pos1 = pos1->next;
    }
    if (total == (2 * deleted))
    {
        // number of deleted elements is same with that of remainders
        pos1 = lst->next;
        pos2 = lst;  // now pos2 represents the previous node of pos1
        while (pos1 != NULL)
        {
            if (pos1->is_deleted == 1)
            {
                pos2->next = pos1->next;
                free(pos1);
                pos1 = pos2->next;
            }
            else
            {
                pos2 = pos1;
                pos1 = pos1->next;
            }
        }
        
    }
    return 0;
}

int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    int number = 10;
    int array[10]; //= {8, 7, 3, 7, 7, 5, 3, 0, 8, 2}(a special case);
    if (create_array(array, number) == -1)
        return -1;
    printf("array: ");
    print_array(array, number);
    t_list lst = (t_list)malloc(sizeof(struct t_node));
    if (lst == NULL)
        return -1;
    init_list(lst);
    create_list(lst, array, number);
    printf("list before del duplicates: ");
    print_list(lst);
    lazy_del_duplicate_list(lst);
    printf("\nlist after del duplicates: ");
    print_list(lst);
    free_list(lst);
}