/*
note:
1. use a t_list variable 'pos_prev' to get the previous element of 'pos', 
    avoiding bidirectional list. this method is useful when looping list
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
    t_position next;
};

int insert(t_list lst, int elem)
{
    ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
    if (tmp_cell == NULL)
        return -1;
    tmp_cell->element = elem;
    tmp_cell->next = lst->next;
    lst->next = tmp_cell;
    return 0;
}

int init_list(t_list lst)
{
    lst->element = 0;
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

int find(t_list lst, int x)
{
    if (lst == NULL)
        return -1;
    t_position pos = lst->next;
    t_position pos_prev = pos; // one way to avoid using bidirectional list
    int id = 0;
    while (pos != NULL)
    {
        if (pos->element == x)
        {
            if (id != 0)
            {
                pos_prev->next = pos->next;
                pos->next = lst->next;
                lst->next = pos;
            }
            return id;
        }
        id++;
        pos_prev = pos;
        pos = pos->next;
    }
    return -1;
}

int create_array(int *array, int len)
{
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    for (int i = 0; i < len; i++)
        array[i] = i;
    for (int i = 1; i < len; i++)
    {
        int r = rand() % i;
        int tmp = array[i];
        array[i] = array[r];
        array[r] = tmp;
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

int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    int number = 10;
    int array[10];
    if (create_array(array, number) != -1)
    {
        printf("array: ");
        print_array(array, number);
        t_list lst = (t_list)malloc(sizeof(struct t_node));
        if (init_list(lst) != -1)
        {
            for (int i = 0; i < number; i++)
                if (insert(lst, array[i]) == -1)
                    return -1;
            printf("list: ");
            if (print_list(lst) == -1)
                return -1;
            for (int i = 0; i < 5; i++)
            {
                int r = rand() % number;
                int id;
                if ((id = find(lst, r)) != -1)
                {
                    printf("the %dth number is: %d \n", id, r);
                    printf("list: ");
                    print_list(lst);
                }
            }
        }
    }
}