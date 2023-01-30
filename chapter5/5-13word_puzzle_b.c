/*
note:
1. note the bounder of for loop
2. when creating prefix hashtable, I have to allocate memory for every prefix string using malloc (line: 122)
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

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

int hash(ElementType key, int table_size)
{
    int hash_val = 0;
    for (int i = 0; *key != '\0'; i++)
        hash_val = (hash_val << 5) + *key++;
    return hash_val % table_size;
}

int find_square(ElementType key, hash_table h)
{
    int collision_num = 0;
    int current_position = hash(key, h->table_size);
    while (h->the_cells[current_position].info != empty && strcmp(h->the_cells[current_position].element, key) != 0)
    {
        current_position += 2 * ++collision_num - 1;
        if (current_position > h->table_size)
            current_position -= h->table_size;
    }
    return current_position;
}

int insert(ElementType key, hash_table h)
{
    if (h == NULL)
        return -1;
    int p = find_square(key, h);
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
    return 0;
}

hash_table create_prefix_hash_dict(char *str[], int str_num, int table_size)
{
    hash_table h = initialize_table(table_size);

    for (int i = 0; i < str_num; i++)
    {
        int len = 0;
        for (char *s = str[i]; *s != '\0'; s++)
            len++;
        
        for (int j = 0; j < len; j++)
        {
            char *str_cell = malloc(sizeof(char) * (len - j + 1));
            if (str_cell == NULL)
                return NULL;
            
            memcpy(str_cell, str[i], (len - j) * sizeof(char));
            str_cell[len - j] = '\0';

            if (insert(str_cell, h) < 0)
                return NULL;
            
        }
    }

    return h;
}

hash_table create_word_hash_dict(char *str[], int str_num, int table_size)
{
    hash_table h = initialize_table(table_size);

    for (int i = 0; i < str_num; i++)
    {
        if (insert(str[i], h) < 0)
            return NULL;
    }
    return h;
}

int word_puzzle(char str[][4], int row, int column, hash_table word_dict, hash_table prefix_dict)
{
    enum direction {left, right, up, down, left_up, right_up, left_down, right_down} dire;
    for (int r = 0; r < row; r++)
    {
        for (int c = 0; c < column; c++)
        {
            // right
            for (int l = 0; c + l < column; l++)
            {
                char s[l + 2];
                for (int i = 0; i <= l; i++)
                    s[i] = str[r][c + i];

                s[l + 1] = '\0';
                int pos = find_square(s, prefix_dict);
                if (pos < 0 || pos >= prefix_dict->table_size)
                {
                    printf("wrong position found!\n");
                    return -1;
                }
                if (prefix_dict->the_cells[pos].info != legitimate)
                    break;
                
                pos = find_square(s, word_dict);
                if (pos < 0 || pos >= word_dict->table_size)
                {
                    printf("wrong position found!\n");
                    return -1;
                }
                if (word_dict->the_cells[pos].info == legitimate)
                    printf("\nword: \"%s\" location: (%d, %d) direction: right", s, r, c);
            }

            // left
            for (int l = c; l >= 0; l--)
            {
                char s[c - l + 2];
                for (int i = c; i >= l; i--)
                {
                    s[c - i] = str[r][i];
                }

                s[c - l + 1] = '\0';

                int pos = find_square(s, prefix_dict);
                if (pos < 0 || pos >= prefix_dict->table_size)
                {
                    printf("wrong position found!\n");
                    return -1;
                }
                if (prefix_dict->the_cells[pos].info != legitimate)
                    break;
                
                pos = find_square(s, word_dict);
                if (pos < 0 || pos >= word_dict->table_size)
                {
                    printf("wrong position found!\n");
                    return -1;
                }
                if (word_dict->the_cells[pos].info == legitimate)
                    printf("\nword: \"%s\" location: (%d, %d) direction: left", s, r, c);
            }

            // down
            for (int l = 0; r + l < row; l++)
            {
                char s[l + 2];
                for (int i = 0; i <= l; i++)
                    s[i] = str[r + i][c];

                s[l + 1] = '\0';
                
                int pos = find_square(s, prefix_dict);
                if (pos < 0 || pos >= prefix_dict->table_size)
                {
                    printf("wrong position found!\n");
                    return -1;
                }
                if (prefix_dict->the_cells[pos].info != legitimate)
                    break;
                
                pos = find_square(s, word_dict);
                if (pos < 0 || pos >= word_dict->table_size)
                {
                    printf("wrong position found!\n");
                    return -1;
                }
                if (word_dict->the_cells[pos].info == legitimate)
                    printf("\nword: \"%s\" location: (%d, %d) direction: down", s, r, c);
            }

            // up
            for (int l = r; l >= 0; l--)
            {
                char s[r - l + 2];
                for (int i = r; i >= l; i--)
                {
                    s[r - i] = str[i][c];
                }

                s[r - l + 1] = '\0';
                
                int pos = find_square(s, prefix_dict);
                if (pos < 0 || pos >= prefix_dict->table_size)
                {
                    printf("wrong position found!\n");
                    return -1;
                }
                if (prefix_dict->the_cells[pos].info != legitimate)
                    break;
                
                pos = find_square(s, word_dict);
                if (pos < 0 || pos >= word_dict->table_size)
                {
                    printf("wrong position found!\n");
                    return -1;
                }
                if (word_dict->the_cells[pos].info == legitimate)
                    printf("\nword: \"%s\" location: (%d, %d) direction: up", s, r, c);
            }
        }
    }
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
    int number = 6;
    char *s[] = {"this", "ih", "fowt", "this", "haa", "ithd"};
    
    hash_table word_dict = create_word_hash_dict(s, number, 101);
    hash_table prefix_dict = create_prefix_hash_dict(s, number, 101);

    printf("\nword hash dict: ");
    print_hash_table(word_dict);
    printf("\nprefix hash dict: ");
    print_hash_table(prefix_dict);

    char words[4][4] = {
        {'t', 'h', 'i', 's'},
        {'w', 'a', 't', 's'},
        {'o', 'a', 'h', 'g'},
        {'f', 'g', 'd', 't'},
    };

    printf("\nfollowing words found: \n");
    word_puzzle(words, 4, 4, word_dict, prefix_dict);
    printf("\n");
    return 0;
}
