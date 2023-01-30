
/*
note:
1. the hashtable is not sorted
2. note to add 'value' in 'hash_entry' to represent value (in this case is coefficient), 'element' is key(in this case is exponent)
*/
#include <stdlib.h>
#include <stdio.h>

#define ElementType u_int32_t
#define MinTableSize 100
#define Key_Size 200

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

struct list_node;
typedef struct list_node *position;
typedef position list;
struct hashtbl;
typedef struct hashtbl *hash_table;

struct list_node
{
    ElementType element;
    position next;
};

struct hash_entry;
typedef struct hash_entry cell;
struct hashtbl;
typedef struct hashtbl *hash_table;

enum kind_of_entry {legitimate, empty, deleted};

struct hash_entry
{
    ElementType element;
    ElementType value;
    enum kind_of_entry info;
};

struct hashtbl
{
    int table_size;
    cell *the_cells;
};

hash_table initialize_table(int table_size)
{
    if (table_size < MinTableSize)
    {
        printf("Table size too small!\n");
        return NULL;
    }
    hash_table h = malloc(sizeof(struct hashtbl));
    if (h == NULL)
    {
        printf("Out of space!\n");
        return NULL;
    }
    
    h->table_size = table_size;
    h->the_cells = malloc(sizeof(cell) * table_size);
    if (h->the_cells == NULL)
    {
        printf("Out of space!\n");
        return NULL;
    }

    for (int i = 0; i < table_size; i++)
    {
        h->the_cells[i].info = empty;
    }
    return h;
}

int hash(u_int32_t key, int table_size)
{
    return key % table_size;
}

int find_square(ElementType key, hash_table h)
{
    int collision_num = 0;
    int current_position = hash(key, h->table_size);
    while (h->the_cells[current_position].info != empty && h->the_cells[current_position].element != key)
    {
        current_position += 2 * ++collision_num - 1;
        if (current_position > h->table_size)
            current_position -= h->table_size;
    }
    return current_position;
}

int print_hash_table(hash_table h)
{
    if (h == NULL)
        return 0;
    for (int i = 0; i < h->table_size; i++)
    {
        if (h->the_cells[i].info == legitimate)
        {
            printf("\nindex %d: key: %d value: %d", i, h->the_cells[i].element, h->the_cells[i].value);
        }
    }
    return 0;
}

int insert_list(t_list l, int coef, int expo, t_position p)
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

int mult_poly(t_list list1, t_list list2, hash_table h)
{
    ptr_to_node pos1 = list1->next;
    ptr_to_node pos2;
    while (pos1 != NULL)
    {
        pos2 = list2->next;
        while (pos2 != NULL)
        {
            int coefficient = pos1->coefficient * pos2->coefficient;
            int exponent = pos1->exponent + pos2->exponent;
            int p = find_square(exponent, h);
            if (h->the_cells[p].info != legitimate)
            {
                h->the_cells[p].info = legitimate;
                h->the_cells[p].element = exponent;
                h->the_cells[p].value = coefficient;
            }
            else
            {
                h->the_cells[p].value += coefficient;
            }
            pos2 = pos2->next;
        }
        pos1 = pos1->next;
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
        insert_list(list1, coefficient_arr1[i], exponent_arr1[i], p);
        p = p->next;
    }
    ptr_to_node list2 = (ptr_to_node)malloc(sizeof(struct t_node));
    list2->coefficient = 0;
    list2->exponent = 0;
    list2->next = NULL;
    p = list2;
    for (int i = 0; i < len2; i++)
    {
        insert_list(list2, coefficient_arr2[i], exponent_arr2[i], p);
        p = p->next;
    }
    printf("\nlist1: \n");
    print_list(list1);
    printf("\nlist2: \n");
    print_list(list2);
    
    hash_table h = initialize_table(101);
    mult_poly(list1, list2, h);
    printf("\npolyprod_list: \n");
    print_hash_table(h);
    free_list(list1);
    free_list(list2);
}
