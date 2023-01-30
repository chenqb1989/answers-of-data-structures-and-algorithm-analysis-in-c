/*
note:
1. note the append time of list: O(len(list))
2. valid only when the number is lower than bucket^round
3. the usage of base and dividor
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

int radix_sort(int * array, int array_len, int buckets, int round)
{
    if (array == NULL || array_len <= 0 || buckets <= 0 || round <= 0)
        return -1;
    t_list lists[buckets];
    int base = 1;
    int dividor = 1;

    for (int r = 0; r < round; r++)
    {
        base *= buckets;
        if (r > 0)
            dividor *= buckets;
        if (create_list_array(lists, buckets) != -1)
        {
            for (int i = 0; i < array_len; i++)
            {
                int bucket = array[i] % base;
                bucket = bucket / dividor;
                t_list lst = lists[bucket];
                if (append(lists[bucket], array[i]) == -1)
                    return -1;
            }
            // update array
            int arr_id = 0;
            for (int i = 0; i < buckets; i++)
            {
                t_list lst = lists[i]->next;
                while (lst != NULL)
                {
                    array[arr_id++] = lst->element;
                    lst = lst->next;
                }
            }
            // clear buckets
            for (int i = 0; i < buckets; i++)
                free_list(lists[i]);
        }
        else
            return -1;
    }
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
    int number = 15000;
    int array[15000];
    int buckets = 1000;
    int round = 3;
    if (create_array(array, number) != -1)
    {
        printf("array before sort: ");
        print_array(array, number);
        if (radix_sort(array, number, buckets, round) != -1)
        {
            printf("\narray after sort: ");
            print_array(array, number);
        }
            
    }
}