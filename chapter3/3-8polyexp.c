/*
note:
1. when computing exp of poly, there are many immediate poly results, note to free them
2. note the difference between 'pointer variable' and 'pointer returned by malloc' 
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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

int init_list(t_list lst)
{
    lst->coefficient = 0;
    lst->exponent = 0;
    lst->next = NULL;
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
    init_list(tmp_list);
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
                same_exponent = 1;
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
    return 0;
}

// int copy_list(t_list list1, t_list list2)
// {
//     ptr_to_node pos1 = list1->next;
//     ptr_to_node pos2 = list2;
//     while (pos1 != NULL)
//     {
//         ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
//         tmp_cell->coefficient = pos1->coefficient;
//         tmp_cell->exponent = pos1->exponent;
//         pos2->next = tmp_cell;
//         pos2 = tmp_cell;
//         pos1 = pos1->next;
//     }
// }

t_list exp_poly(t_list poly, int exp_num)
{
    assert(exp_num > 0);
    ptr_to_node poly_mul1 = poly;
    ptr_to_node poly_mul2 = poly;

    for (int i = 0; i < exp_num - 1; i++)
    {
        ptr_to_node poly_prod = (ptr_to_node)malloc(sizeof(struct t_node));
        init_list(poly_prod);
        mult_poly(poly_mul1, poly_mul2, poly_prod);
        if (i > 0)
            free_list(poly_mul2); // free immediate result
        poly_mul2 = poly_prod;
    }
    return poly_mul2;
}

t_list square_poly(t_list poly, int isfree)
{
    t_list poly_square = (ptr_to_node)malloc(sizeof(struct t_node));
    init_list(poly_square);
    mult_poly(poly, poly, poly_square);
    if (isfree)
        free_list(poly);
    return poly_square;
}

t_list exp_poly_fast(t_list poly, int exp_num)
{
    assert(exp_num > 0);
    if (exp_num == 1)
        return poly;
    else if (exp_num == 2)
    {
        return square_poly(poly, 0);
    }
    else if (exp_num % 2 == 0)
    {
        t_list tmp = exp_poly_fast(poly, exp_num / 2);
        return square_poly(tmp, 1);
    }
    else if (exp_num % 2 == 1)
    {
        t_list tmp = exp_poly_fast(poly, exp_num -1);
        t_list poly_prod = (ptr_to_node)malloc(sizeof(struct t_node));
        init_list(poly_prod);
        mult_poly(tmp, poly, poly_prod);
        return poly_prod;
    }


}

int main()
{
    const int len1 = 6;
    const int coefficient_arr1[6] = {3, 7, -11, 15, -20, 23};
    const int exponent_arr1[6] = {2000, 1800, 1300, 1000, 300, 5};

    ptr_to_node list1 = (ptr_to_node)malloc(sizeof(struct t_node));
    init_list(list1);
    ptr_to_node p = list1;
    for (int i = 0; i < len1; i++)
    {
        insert(list1, coefficient_arr1[i], exponent_arr1[i], p);
        p = p->next;
    }
    printf("\nlist1: \n");
    print_list(list1);

    // ptr_to_node result = exp_poly_fast(list1, 6);
    ptr_to_node result = exp_poly(list1, 6);
    printf("\npolyprod_list: \n");
    print_list(result);
    free_list(list1);
    free_list(result);
}
