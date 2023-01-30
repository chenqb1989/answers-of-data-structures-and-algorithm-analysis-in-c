/*
note:
1. note that we only store pointer to string in 'hash_entry', 
    so in 'hash' and 'find_square' functions, we can't use '\0' to indicate string end, 
    and we should use string length to indicate the end of string
2. 
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType char *
#define MinTableSize 100


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

int hash(ElementType key, int table_size, int str_len)
{
    int hash_val = 0;
    for (int i = 0; *key != '\0' && i < str_len; i++)
        hash_val = (hash_val << 5) + *key++;
    return hash_val % table_size;
}

// compare begining str_len characters of s1 and s2
int my_strcmp(char *s1, char *s2, int str_len)
{
    int i;
    for (i = 0; *s1 != '\0' && *s2 != '\0' && i < str_len; i++)
    {
        if (*s1 != *s2)
            return 0;
        s1++;
        s2++;
    }
    if (i == str_len)
        return 1;
    else
        return 0;
}

int find_square(ElementType key, hash_table h, int str_len)
{
    int collision_num = 0;
    int current_position = hash(key, h->table_size, str_len);
    int flag = h->the_cells[current_position].info != empty;
    while (h->the_cells[current_position].info != empty && my_strcmp(h->the_cells[current_position].element, key, str_len) == 0)
    {
        current_position += 2 * ++collision_num - 1;
        if (current_position > h->table_size)
        {
            current_position -= h->table_size;
        }
    }
    return current_position;
}

int insert(ElementType key, hash_table h, int str_len)
{
    if (h == NULL)
        return -1;
    int p = find_square(key, h, str_len);
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
            printf("\nindex %d: %s", i, h->the_cells[i].element);
        }
    }
    return 1;
}

hash_table create_pattern_string(char *str, char *pattern, int table_size)
{
    hash_table h = initialize_table(table_size);
    
    int pattern_len = 0;
    for (char *p = pattern; *p != '\0'; p++)
        pattern_len++;

    for (char *s = str; *(s + pattern_len) != '\0'; s++)
    {
        if (insert(s, h, pattern_len) < 0)
            return NULL;
    }
    return h;
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
        printf("%c ", array[i]);
    printf("\n");
    return 0;
}

int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    char *s = "he is a good man";
    char *pattern = "ma";
    int pattern_len = 0;
    for (char *p = pattern; *p != '\0'; p++)
        pattern_len++;
    
    hash_table h = create_pattern_string(s, pattern, 101);
    print_hash_table(h);
    int index = find_square(pattern, h, pattern_len);
    if (h->the_cells[index].info == legitimate)
        printf("\nindex: %d, value: %s\n", index, h->the_cells[index].element);
    else
        printf("\nthe pattern: %s not found!\n", pattern);

    return 0;
}
