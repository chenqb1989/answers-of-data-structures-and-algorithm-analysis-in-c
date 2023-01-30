#include <stdlib.h>
#include <stdio.h>

struct t_node;
typedef struct t_node *ptr_to_node;
typedef ptr_to_node t_list;
typedef ptr_to_node t_position;

struct t_node
{
    int coefficient;
    int exponent;

    t_position next;
};

int insert(t_list l, int coef, int expo, t_position p)
{
    ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
    if (tmp_cell == NULL)
        return -1;
    tmp_cell->coefficient = coef;
    tmp_cell->exponent = expo;
    tmp_cell->next = p->next;
    p->next = tmp_cell;
    return 0;
}

int print_list(t_list l)
{
    if (l == NULL)
        return 0; // empty list
    ptr_to_node p = l->next;
    while (p != NULL)
    {
        printf("coefficient:%d, exponent:%d\n", p->coefficient, p->exponent);
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

int mult_poly(t_list list1, t_list list2, t_list poly_prod)
{
    ptr_to_node pos1 = list1->next;
    ptr_to_node pos2;
    ptr_to_node tmp_list = (ptr_to_node)malloc(sizeof(struct t_node));
    tmp_list->coefficient = 0;
    tmp_list->exponent = 0;
    tmp_list->next = NULL;
    ptr_to_node pos_tmp = tmp_list;
    while (pos1 != NULL)
    {
        pos2 = list2->next;
        while (pos2 != NULL)
        {
            ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
            tmp_cell->coefficient = pos1->coefficient * pos2->coefficient;
            tmp_cell->exponent = pos1->exponent + pos2->exponent;
            tmp_cell->next = pos_tmp->next;
            pos_tmp->next = tmp_cell;
            pos_tmp = tmp_cell;
            pos2 = pos2->next;
        }
        pos1 = pos1->next;
    }
    pos_tmp = tmp_list->next;
    ptr_to_node pos_poly;
    while (pos_tmp != NULL)
    {
        int same_exponent = 0;
        pos_poly = poly_prod;
        int tmp_coefficient = pos_tmp->coefficient;
        int tmp_exponent = pos_tmp->exponent;
        while (pos_poly->next != NULL)
        {
            pos_poly = pos_poly->next;
            if (pos_poly->exponent == tmp_exponent)
            {
                pos_poly->coefficient += tmp_coefficient;
                break;
            }
        }
        if (same_exponent == 0)
        {
            ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
            tmp_cell->coefficient = tmp_coefficient;
            tmp_cell->exponent = tmp_exponent;
            tmp_cell->next = pos_poly->next;
            pos_poly->next = tmp_cell;
        }
        pos_tmp = pos_tmp->next;
    }
    free_list(tmp_list);
}

int main()
{
    const int len1 = 6;
    const int coefficient_arr1[6] = {3, 7, -11, 15, -20, 23};
    const int exponent_arr1[6] = {2000, 1800, 1300, 1000, 300, 5};
    const int len2 = 8;
    const int coefficient_arr2[8] = {87, 17, 11, -3, 10, 23, -55, 90};
    const int exponent_arr2[8] = {2000, 1700, 1500, 1300, 300, 20, 8, 2};

    ptr_to_node list1 = (ptr_to_node)malloc(sizeof(struct t_node));
    list1->coefficient = 0;
    list1->exponent = 0;
    list1->next = NULL;
    ptr_to_node p = list1;
    for (int i = 0; i < len1; i++)
    {
        insert(list1, coefficient_arr1[i], exponent_arr1[i], p);
        p = p->next;
    }
    ptr_to_node list2 = (ptr_to_node)malloc(sizeof(struct t_node));
    list2->coefficient = 0;
    list2->exponent = 0;
    list2->next = NULL;
    p = list2;
    for (int i = 0; i < len2; i++)
    {
        insert(list2, coefficient_arr2[i], exponent_arr2[i], p);
        p = p->next;
    }
    printf("\nlist1: \n");
    print_list(list1);
    printf("\nlist2: \n");
    print_list(list2);
    ptr_to_node result = (ptr_to_node)malloc(sizeof(struct t_node));
    result->coefficient = 0;
    result->exponent = 0;
    result->next = NULL;
    mult_poly(list1, list2, result);
    printf("\npolyprod_list: \n");
    print_list(result);
    free_list(list1);
    free_list(list2);
    free_list(result);
}
