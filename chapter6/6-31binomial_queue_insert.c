/*
note:
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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

// return the result of merging equal-sized t1 and t2
bin_tree combine_trees(bin_tree t1, bin_tree t2)
{
    if (t1->element > t2->element)
        return combine_trees(t2, t1);
    t2->next_sibling = t1->left_child;
    t1->left_child = t2;
    return t1;
}

int insert(bin_queue h, ElementType x)
{
    if (h == NULL)
        return -1;

    bin_tree carry = create_node(x);
    if (carry == NULL)
        return -1;
    if (h->current_size + 1 > Capacity)
        return -1;

    h->current_size++;
    int i, j;
    for (i = 0, j = 1; j <= h->current_size; i++, j *= 2)
    {
        if (h->the_trees[i] && carry)
        {
            carry = combine_trees(h->the_trees[i], carry);
            h->the_trees[i] = NULL;
        }
        else if (carry)
        {
            h->the_trees[i] = carry;
            break;
        }
        else
        {
            break;
        }
    }
    return 0;
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
    srand(time(NULL)); // Initialization, should only be called once.
    int number = 10;
    int array[number];
    create_array(array, number);
    printf("array:\n");
    print_array(array, number);

    bin_queue h = create_queue();
    if (h == NULL)
        return -1;

    for (int i = 0; i < number; i++)
        insert(h, array[i]);
    printf("bin_queue using insert:\n");
    print_bin_queue(h);
    free_bin_queue(h);

    printf("\n");
    return 0;
}