/*
note:
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType u_int32_t
#define MinTableSize 100

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

struct hashtbl
{
    int table_size;
    list *the_lists;
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
    h->the_lists = malloc(sizeof(list) * table_size);
    if (h->the_lists == NULL)
    {
        printf("Out of space!\n");
        return NULL;
    }

    list the_lists = malloc(sizeof(struct list_node) * table_size);
    if (the_lists == NULL)
    {
        printf("Out of space!\n");
        return NULL;
    }


    for (int i = 0; i < table_size; i++)
    {
        list l;
        l = &the_lists[i];
        l->next = NULL;
        h->the_lists[i] = l;
    }
    return h;
}

int hash(u_int32_t key, int table_size)
{
    // int hash_val = 0;
    // while (*key != '\0')
    //     hash_val = (hash_val << 5) + *key++;
    return key % table_size;
}

position find(ElementType key, hash_table h)
{
    list l = h->the_lists[hash(key, h->table_size)];
    position p = l->next;
    while (p != NULL && p->element != key)
        p = p->next;
    return p;
}

int insert(ElementType key, hash_table h)
{
    if (h == NULL)
        return -1;
    position p = find(key, h);
    if (p == NULL)
    {
        position tmp_cell = malloc(sizeof(struct list_node));
        if (tmp_cell == NULL)
        {
            printf("Out of space!\n");
            return -1;
        }
        tmp_cell->element = key;
        position start_pos = h->the_lists[hash(key, h->table_size)];
        tmp_cell->next = start_pos->next;
        start_pos->next = tmp_cell;
    }
    return 0;
}

int print_hash_table(hash_table h)
{
    if (h == NULL)
        return 0;
    for (int i = 0; i < h->table_size; i++)
    {
        position p = h->the_lists[i]->next;
        if (p != NULL)
        {
            printf("\nindex %d:", i);
            while (p != NULL)
            {
                printf("%d ", p->element);
                p = p->next;
            }
        }
    }
    return 0;
}

int initialize_array(int *array, int len, int max_value)
{
    for (int i = 0; i < len; i++)
    {
        int r = rand() % max_value;
        array[i] = r;
    }
    return 0;
}

int print_array(int *array, int len)
{
    for (int i = 0; i < len; i++)
        printf("%d ", array[i]);
    printf("\n");
    return 0;
}

int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    hash_table h = initialize_table(100);
    int number = 10;
    int max_value = 1000;
    int array[number];
    initialize_array(array, number, max_value);
    printf("array:\n");
    print_array(array, number);
    
    for (int i = 0; i < number; i++)
    {
        insert(array[i], h);
    }
    printf("\nhash table:\n");
    print_hash_table(h);
    printf("\n");

    return 0;
}
