/*
note:
1. like 9-9dijkstra.c, we use struct object(character_struct) as element of a heap
2. set_encoder use a char array, when entering a node, appending '0' or '1' to the array, when leaving, deleting last character
3. decode need using the huffman tree
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define ElementType character
#define MaxValue __INT32_MAX__
#define MinPQSize 1
#define MinData -1
#define MinTableSize 100
#define MaxEoncodeLen 20

/* hash table */
struct hash_entry;
typedef struct hash_entry cell;
struct hashtbl;
typedef struct hashtbl *hash_table;

enum kind_of_entry {legitimate, empty, deleted};

struct hash_entry
{
    char element;
    int value;
    char encoder[MaxEoncodeLen];
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

int hash(char key, int table_size)
{
    return key % table_size;
}

int find_square(char key, hash_table h)
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

int insert_hash(char key, int value, hash_table h)
{
    if (h == NULL)
        return -1;
    int p = find_square(key, h);
    if (h->the_cells[p].info != legitimate)
    {
        h->the_cells[p].info = legitimate;
        h->the_cells[p].element = key;
        h->the_cells[p].value = value;
    }
    return 0;
}

int in_hashtable(char key, hash_table h)
{
    int p = find_square(key, h);
    if (h->the_cells[p].info != legitimate)
        return 0;
    else
        return 1;
}

/* priority queue */
struct character_struct;
typedef struct character_struct *character;
struct character_struct
{
    char cha;
    int weight;
    character left;
    character right;
};

struct heap_struct;
typedef struct heap_struct *priority_queue;

struct heap_struct
{
    int capacity;
    int size;
    ElementType *elements;
};

priority_queue initialize(int max_elments)
{
    priority_queue h;

    if (max_elments < MinPQSize)
    {
        printf("priority queue size is too small\n");
        return NULL;
    }

    h = malloc(sizeof(struct heap_struct));
    if (h == NULL)
    {
        printf("out of space\n");
        return NULL;
    }

    h->elements = malloc(sizeof(ElementType) * (max_elments + 1));
    if (h->elements == NULL)
    {
        printf("out of space\n");
        return NULL;
    }

    h->capacity = max_elments;
    h->size = 0;
    h->elements[0] = malloc(sizeof(struct character_struct));
    if (h->elements[0] == NULL)
    {
        printf("out of space\n");
        return NULL;
    }
    h->elements[0]->weight = MinData;
    return h;
}

int is_full(priority_queue h)
{
    if (h->size == h->capacity)
        return 1;
    else
        return 0;
}

int is_empty(priority_queue h)
{
    if (h->size == 0)
        return 1;
    else
        return 0;
}

int insert(ElementType x, priority_queue h)
{
    if (is_full(h))
    {
        printf("the priority queue is full\n");
        return -1;
    }

    int i = ++h->size;
    for (; h->elements[i / 2]->weight > x->weight; i /= 2)
        h->elements[i] = h->elements[i / 2];
    h->elements[i] = x;
    return 0;
}

ElementType delete_min(priority_queue h)
{
    if (is_empty(h))
    {
        printf("the priority queue is empty!\n");
        return h->elements[0];
    }

    ElementType min_element = h->elements[1];
    ElementType last_element = h->elements[h->size--];
    int child, i;
    for (i = 1; i * 2 <= h->size; i = child) // note the termination condition
    {
        child = i * 2;
        if (child != h->size && h->elements[child + 1]->weight < h->elements[child]->weight)
            child++;
        
        if (last_element->weight > h->elements[child]->weight)
            h->elements[i] = h->elements[child];
        else
            break;
    }
    h->elements[i] = last_element;
    return min_element;
}

void percolate_down(priority_queue h, int index)
{
    int child, i;
    for (i = index; i * 2 <= h->size; i = child)
    {
        child = i * 2;
        if (child != h->size && h->elements[child + 1]->weight < h->elements[child]->weight)
            child++;
        
        if (h->elements[i]->weight > h->elements[child]->weight)
        {
            ElementType temp = h->elements[i];
            h->elements[i] = h->elements[child];
            h->elements[child] = temp;
        }
        else
            break;
    }
}

priority_queue build_heap(character chars, int cha_num)
{
    priority_queue h = initialize(cha_num);
    
    for (int i = 0; i < cha_num; i++)
        h->elements[i + 1] = &chars[i];
    h->size = cha_num;
    for (int i = h->size / 2; i > 0; i--)
        percolate_down(h, i);
    
    return h;
}

int print_priority_queue(priority_queue h)
{
    if (h == NULL)
        return -1;
    
    for (int i = 1; i <= h->size; i++)
        printf("%c_%d ", h->elements[i]->cha, h->elements[i]->weight);
    return 0; 
}

/* huffman code */
int compute_freq(char *text, hash_table h)
{
    int count = 0;
    char *s = text;
    while (*s != '\0')
    {
        if (in_hashtable(*s, h) == 0)
        {
            insert_hash(*s, 1, h);
            count++;
        }
        else
        {
            int p = find_square(*s, h);
            h->the_cells[p].value += 1;
        }
        s++;
    }
    return count;
}

character create_huffman_tree(char *text, hash_table h)
{
    int cha_num = compute_freq(text, h);
    character cha_freq = malloc(sizeof(struct character_struct) * cha_num);
    if (cha_freq == NULL)
        return NULL;
    
    // copy value from hash_table to cha_freq
    int j = 0;
    character temp = cha_freq;
    for (int i = 0; i < h->table_size; i++)
    {
        if (h->the_cells[i].info == legitimate)
        {            
            temp->cha = h->the_cells[i].element;
            temp->weight = h->the_cells[i].value;
            temp->left = NULL;
            temp->right = NULL;
            temp++;
            j++;
        }
    }
    if (j != cha_num)
    {
        printf("something is wrong");
        return NULL;
    }

    // build heap
    priority_queue heap = build_heap(cha_freq, cha_num);
    // print_priority_queue(heap);

    // create huffman code tree
    while (heap->size != 1)
    {
        character left = delete_min(heap);
        character right = delete_min(heap);

        // merge
        character father = malloc(sizeof(struct character_struct));
        if (father == NULL)
        {
            printf("out of space\n");
            return NULL;
        }
        father->left = left;
        father->right = right;
        insert(father, heap);
    }
    return heap->elements[1];
}

void set_encoder(character tree, hash_table h, char *s)
{
    if (tree->left == NULL && tree->right == NULL)
    {
        int p = find_square(tree->cha, h);
        memcpy(h->the_cells[p].encoder, s, strlen(s));
        h->the_cells[p].encoder[strlen(s)] = '\0';
        printf("\n%c: %s", h->the_cells[p].element, h->the_cells[p].encoder);
    }
    else
    {
        s[strlen(s)] = '0';
        s[strlen(s)] = '\0';
        set_encoder(tree->left, h, s);
        s[strlen(s)] = '1';
        s[strlen(s)] = '\0';
        set_encoder(tree->right, h, s);
    }
    s[strlen(s) - 1] = '\0';
}

void encode(char *text, hash_table h)
{
    char result[strlen(text) * MaxEoncodeLen];
    char *end = result;
    char *s = text;
    while (*s != '\0')
    {
        int p = find_square(*s, h);
        char *e = h->the_cells[p].encoder;
        while (*e != '\0')
        {
            *end = *e;
            end++;
            e++;
        }
        *end = '\0';
        s++;
    }
    printf("\nencoding of %s: %s", text, result);
}

void decode(char *text, character tree, character root)
{

    if (tree->left == NULL && tree->right == NULL)
    {
        printf("%c", tree->cha);
        if (*text != '\0')
            decode(text, root, root);
    }
    else
    {
        if (*text == '0')
            decode(++text, tree->left, root);
        else
            decode(++text, tree->right, root);
    }
        
}

int main()
{
    char *text = "huffmauuewduaada";

    hash_table h = initialize_table(MinTableSize);
    if (h == NULL)
        return -1;
    character tree = create_huffman_tree(text, h);
    char s[MaxEoncodeLen] = "";
    printf("\nencoder: ");
    set_encoder(tree, h, s);
    encode(text, h);
    char *encode = "000000101001001000011010100000000000010001011100011";
    printf("\ndecoding of 000000101001001000011010100000000000010001011100011: ");
    decode(encode, tree, tree);
    printf("\n");
    return 0;
}