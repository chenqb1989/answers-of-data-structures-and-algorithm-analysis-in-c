/*
note:
1. I tested three detection methods, using different insert numbers,
    in this situation, square detection and double hash is better than linear detection
2. note to substract table_size when current_position is greater than table_size
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType u_int32_t
#define MinTableSize 100

int total_collision_num = 0;

struct hash_entry;
typedef struct hash_entry cell;
struct hashtbl;
typedef struct hashtbl *hash_table;

enum kind_of_entry {legitimate, empty, deleted};

struct hash_entry
{
    ElementType element;
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
    // int hash_val = 0;
    // while (*key != '\0')
    //     hash_val = (hash_val << 5) + *key++;
    return key % table_size;
}

int hash2(u_int32_t key, int table_size)
{
    return table_size - (key % table_size);
}

int find_linear(ElementType key, hash_table h)
{
    int collision_num = 0;
    int current_position = hash(key, h->table_size);
    while (h->the_cells[current_position].info != empty && h->the_cells[current_position].element != key)
    {
        collision_num++;
        if (++current_position > h->table_size)
            current_position = 0;
    }
    total_collision_num += collision_num;
    return current_position;
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
    total_collision_num += collision_num;
    return current_position;
}

int find_double_hash(ElementType key, hash_table h)
{
    int collision_num = 0;
    int current_position = hash(key, h->table_size);
    while (h->the_cells[current_position].info != empty && h->the_cells[current_position].element != key)
    {
        collision_num++;
        current_position += hash2(key, 7);
        if (current_position > h->table_size)
            current_position -= h->table_size;
    }
    total_collision_num += collision_num;
    return current_position;
}

int insert(ElementType key, hash_table h)
{
    if (h == NULL)
        return -1;
    int p = find_double_hash(key, h);
    if (h->the_cells[p].info != legitimate)
    {
        h->the_cells[p].info = legitimate;
        h->the_cells[p].element = key;
    }
    return 0;
}

int print_hash_table(hash_table h)
{
    if (h == NULL)
        return 0;
    for (int i = 0; i < h->table_size; i++)
    {
        if (h->the_cells[i].info == legitimate)
        {
            printf("\nindex %d: %d", i, h->the_cells[i].element);
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
    hash_table h = initialize_table(101);
    int max_value = 1000;
    int numbers[5] = {10, 30, 50, 70, 90};
    int steps = 10;
    for (int i = 0; i < 5; i++)
    {
        int number = numbers[i];
        int array[number];

        for (int j = 0; j < steps; j++)
        {
            initialize_array(array, number, max_value);
            for (int k = 0; k < number; k++)
            {
                insert(array[k], h);
            }
            h = initialize_table(101);
        }
        float average_total_collision_num = (float)total_collision_num / steps;
        printf("\nnumber: %d, total_collision_num: %f\n", number, average_total_collision_num);
        total_collision_num = 0;
    }
    // int number = 70;
    // int max_value = 1000;
    // int array[number];
    // initialize_array(array, number, max_value);
    // printf("array:\n");
    // print_array(array, number);
    
    // for (int i = 0; i < number; i++)
    // {
    //     insert(array[i], h);
    // }
    // printf("\nhash table:\n");
    // print_hash_table(h);
    // printf("\n");
    // printf("\ntotal_collision_num: %d\n", total_collision_num);

    return 0;
}
