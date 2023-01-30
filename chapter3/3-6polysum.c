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
    ptr_to_node p = l->next;
    while (p != NULL)
    {
        printf("coefficient:%d, exponent:%d\n", p->coefficient, p->exponent);
        p = p->next;
    }
    return 0;
}

int add_poly(t_list list1, t_list list2, t_list poly_sum)
{
    ptr_to_node pos1 = list1->next;
    ptr_to_node pos2 = list2->next;
    ptr_to_node pos_sum = poly_sum;
    while (pos1 != NULL || pos2 != NULL)
    {
        if (pos1 == NULL)
        {
            ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
            tmp_cell->coefficient = pos2->coefficient;
            tmp_cell->exponent = pos2->exponent;
            tmp_cell->next = pos_sum->next;
            pos_sum->next = tmp_cell;
            pos_sum = tmp_cell;
            pos2 = pos2->next;
        }
        else if (pos2 == NULL)
        {
            ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
            tmp_cell->coefficient = pos1->coefficient;
            tmp_cell->exponent = pos1->exponent;
            tmp_cell->next = pos_sum->next;
            pos_sum->next = tmp_cell;
            pos_sum = tmp_cell;
            pos1 = pos1->next;
        }
        else
        {
            int exponent1 = pos1->exponent;
            int exponent2 = pos2->exponent;
            if (exponent1 > exponent2)
            {
                ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
                tmp_cell->coefficient = pos1->coefficient;
                tmp_cell->exponent = pos1->exponent;
                tmp_cell->next = pos_sum->next;
                pos_sum->next = tmp_cell;
                pos_sum = tmp_cell;
                pos1 = pos1->next;
            }
            else if (exponent1 < exponent2)
            {
                ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
                tmp_cell->coefficient = pos2->coefficient;
                tmp_cell->exponent = pos2->exponent;
                tmp_cell->next = pos_sum->next;
                pos_sum->next = tmp_cell;
                pos_sum = tmp_cell;
                pos2 = pos2->next;
            }
            else
            {
                ptr_to_node tmp_cell = (ptr_to_node)malloc(sizeof(struct t_node));
                tmp_cell->coefficient = pos1->coefficient + pos2->coefficient;
                tmp_cell->exponent = pos2->exponent;
                tmp_cell->next = pos_sum->next;
                pos_sum->next = tmp_cell;
                pos_sum = tmp_cell;
                pos1 = pos1->next;
                pos2 = pos2->next;
            }
        }
    }
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
    add_poly(list1, list2, result);
    printf("\npolysum_list: \n");
    print_list(result);
}
