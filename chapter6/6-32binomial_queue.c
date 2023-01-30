/*
note:
1. note how to copy a bin_queue (line: 58-95)
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define ElementType int
#define MaxTrees 100
#define Capacity 1000

struct bin_node;
struct collection;
typedef struct bin_node *position;
typedef struct bin_node *bin_tree;
typedef struct collection *bin_queue;

struct bin_node
{
    position left_child;
    position next_sibling;
    ElementType element;
};

struct collection
{
    int current_size;
    bin_tree the_trees[MaxTrees];
};

position create_node(ElementType x)
{
    position node = malloc(sizeof(struct bin_node));
    if (node == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    node->element = x;
    node->left_child = node->next_sibling = NULL;
    return node;
}

bin_queue create_queue()
{
    bin_queue h = malloc(sizeof(struct collection));
    if (h == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    h->current_size = 0;
    return h;
}

bin_tree copy_tree(bin_tree t1)
{
    if (t1 == NULL)
        return NULL;
    
    bin_tree t2 = create_node(t1->element);
    if (t2 == NULL)
        return NULL;

    if (t1->left_child)
        t2->left_child = copy_tree(t1->left_child);
    else
        t2->left_child = NULL;
    
    if (t1->next_sibling)
        t2->next_sibling = copy_tree(t1->next_sibling);
    else
        t2->next_sibling = NULL;
    return t2;
}

bin_queue copy_queue(bin_queue h1)
{
    if (h1 == NULL)
        return NULL;
    
    bin_queue h2 = create_queue();
    if (h2 == NULL)
        return NULL;
    
    int i, j;
    for (i = 0, j = 1; j <= h1->current_size; i++, j *= 2)
    {
        h2->the_trees[i] = copy_tree(h1->the_trees[i]);
    }
    h2->current_size = h1->current_size;
    return h2;
}

// return the result of merging equal-sized t1 and t2
bin_tree combine_trees(bin_tree t1, bin_tree t2)
{
    if (t1->element > t2->element)
        return combine_trees(t2, t1);
    t2->next_sibling = t1->left_child;
    t1->left_child = t2;
    return t1;
}

bin_tree combine_trees_reverse(bin_tree t1, bin_tree t2)
{
    if (t1->element < t2->element)
        return combine_trees_reverse(t2, t1);
    t2->next_sibling = t1->left_child;
    t1->left_child = t2;
    return t1;
}

int print_bin_tree(bin_tree t)
{
    if (t == NULL)
        return 0;
    printf("%d ", t->element);
    print_bin_tree(t->left_child);
    print_bin_tree(t->next_sibling);
}

int print_bin_queue(bin_queue h)
{
    if (h == NULL)
        return 0;
    
    int i, j;
    for (i = 0, j = 1; j <= h->current_size; i++, j *= 2)
    {
        printf("\n the %dth tree: ", i);
        print_bin_tree(h->the_trees[i]);
    }
}

bin_queue merge(bin_queue h1, bin_queue h2)
{
    bin_tree t1, t2, carry = NULL;
    int i, j;

    if (h1->current_size + h2->current_size > Capacity)
    {
        printf("merge would exceed capacity!\n");
        return NULL;
    }

    // 6-32.a
    if (h1 == h2)
    {
        h2 = copy_queue(h1);
        if (h2 == NULL)
            return NULL;
    }

    h1->current_size += h2->current_size;
    for (i = 0, j = 1; j <= h1->current_size; i++, j *= 2)
    {
        // 6-32.b
        if (j > h2->current_size && carry == NULL)
            break;
        
        t1 = h1->the_trees[i];
        t2 = h2->the_trees[i];

        int condition = !!t1 + 2 * !!t2 + 4 * !!carry;
        switch (condition)
        {
        case 0:
        case 1:
            break;
        case 2:
            h1->the_trees[i] = h2->the_trees[i];
            h2->the_trees[i] = NULL;
            break;
        case 3:
            carry = combine_trees_reverse(t1, t2);  // 6-32.c
            h1->the_trees[i] = NULL;
            h2->the_trees[i] = NULL;
            break;
        case 4:
            h1->the_trees[i] = carry;
            carry = NULL;
            break;
        case 5:
            carry = combine_trees_reverse(t1, carry);  // 6-32.c
            h1->the_trees[i] = NULL;
            break;
        case 6:
            carry = combine_trees_reverse(t2, carry);  // 6-32.c
            h2->the_trees[i] = NULL;
            break;
        case 7:
            h1->the_trees[i] = carry;
            carry = combine_trees_reverse(t1, t2);  // 6-32.c
            h2->the_trees[i] = NULL;
            break;
        }
    }
    return h1;
}

void free_bin_tree(bin_tree t)
{
    if (t != NULL)
    {
        free_bin_tree(t->left_child);
        free_bin_tree(t->next_sibling);
        free(t);
    }
}

void free_bin_queue(bin_queue h)
{
    if (h != NULL)
        free(h);
}

int create_array(int *array, int len)
{
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    for (int i = 0; i < len; i++)
        array[i] = i;
    for (int i = 0; i < len; i++)
    {
        int r = rand() % len;
        int tmp = array[i];
        array[i] = array[r];
        array[r] = tmp;
        // array[i] = r;
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
    // srand(time(NULL)); // Initialization, should only be called once.
    int number = 10;
    int array[number];
    create_array(array, number);
    printf("array:\n");
    print_array(array, number);

    bin_queue h = create_queue();
    if (h == NULL)
        return -1;
    
    bin_queue temp = create_queue();
    if (temp == NULL)
        return -1;
    temp->current_size++;
    for (int i = 0; i < number; i++)
    {
        bin_tree t = create_node(0);
        if (t == NULL)
            return -1;
        temp->the_trees[0] = t;
        t->element = array[i];
        h = merge(h, temp);
    }

    printf("\nbin_queue using merge:\n");
    print_bin_queue(h);

    bin_queue h2 = merge(h, h);
    printf("\nmerge two h:\n");
    print_bin_queue(h2);

    free_bin_queue(h2);
    free_bin_queue(temp);

    printf("\n");
    return 0;
}