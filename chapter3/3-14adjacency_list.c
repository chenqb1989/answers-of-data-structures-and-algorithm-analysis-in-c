
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

int insert(t_list lst, int elem, t_list pos)
{
    ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
    if (tmp_cell == NULL)
        return -1;
    tmp_cell->element = elem;
    tmp_cell->next = pos->next;
    pos->next = tmp_cell;
    pos = pos->next;
    return 0;
}

int append(t_list lst, int elem)
{
    if (lst == NULL)
        return -1;
    t_position pos = lst;
    while (pos->next != NULL)
        pos = pos->next;
    t_position tmp_cell = (t_position)malloc(sizeof(struct t_node));
    tmp_cell->element = elem;
    tmp_cell->next = pos->next;
    pos->next = tmp_cell;
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

int create_list_array(t_list * lists, int list_n)
{
    if (lists == NULL || list_n <= 0)
        return -1;
    for (int i = 0; i < list_n; i++)
    {
        t_list lst = (t_list)malloc(sizeof(struct t_node));
        init_list(lst);
        lists[i] = lst;
    }
}

int read_to_list(t_list * lists, int *array, int row, int column, int len)
{
    if (row * column != len)
        return -1;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if (append(lists[i], array[i * column + j]) == -1)
                return -1;
        }
    }
    return 0;
}

int print_lists(t_list * lists, int list_n)
{
    if (lists == NULL)
        return -1;
    for (int i = 0; i < list_n; i++)
    {
        printf("\nthe %dth list: ", i);
        print_list(lists[i]);
    }
    printf("\n");
    return 0;
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
    int number = 100;
    int array[100];
    int node_len = 10;
    if (create_array(array, number) != -1)
    {
        printf("array:\n");
        print_array(array, number);
        t_list lists[10];
        if (create_list_array(lists, node_len) != -1)
        {
            if (read_to_list(lists, array, node_len, number / node_len, number) != -1)
                {
                    print_lists(lists, node_len);
                    for (int i = 0; i < node_len; i++)
                        free_list(lists[i]);
                }
        }
    }
}