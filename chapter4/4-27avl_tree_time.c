/*
note:
1. most time is spent on 'insert', 'find' consumes little time
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int rotate_num = 0;

struct avl_node;
typedef struct avl_node *position;
typedef struct avl_node *avl_tree;

struct avl_node
{
    ElementType element;
    avl_tree left;
    avl_tree right;
    int height;
};

static int get_height(position p)
{
    if (p == NULL)
        return -1;
    else
        return p->height;
}

avl_tree create_tree()
{
    avl_tree tree = malloc(sizeof(struct avl_node));
    if (tree == NULL)
    {
        printf("out of space\n");
        return NULL;
    }
    return tree;
}

avl_tree make_empty(avl_tree tree)
{
    if (tree != NULL)
    {
        make_empty(tree->left);
        make_empty(tree->right);
        tree = NULL;
    }
    return NULL;
}

position find(ElementType x, avl_tree tree)
{
    if (tree == NULL)
        return NULL;
    else if (x < tree->element)
        return find(x, tree->left);
    else if (x > tree->element)
        return find(x, tree->right);
    else
        return tree;
}

position find_min(avl_tree tree)
{
    if (tree == NULL)
        return NULL;
    else if (tree->left != NULL)
        return find_min(tree->left);
    else
        return tree;
}

position find_max(avl_tree tree)
{
    if (tree == NULL)
        return NULL;
    else if (tree->right != NULL)
        return find_max(tree->right);
    else
        return tree;
}

static position single_rotate_with_left(position node)
{
    rotate_num += 1;
    position tmp_cell = node->left;
    node->left = tmp_cell->right;
    tmp_cell->right = node;

    // update height
    int height_l = get_height(node->left);
    int height_r = get_height(node->right);
    node->height = MAX(height_l, height_r) + 1;

    height_l = get_height(tmp_cell->left);
    height_r = get_height(tmp_cell->right);
    tmp_cell->height = MAX(height_l, height_r) + 1;
    return tmp_cell;
}

static position single_rotate_with_right(position node)
{
    rotate_num += 1;
    position tmp_cell = node->right;
    node->right = tmp_cell->left;
    tmp_cell->left = node;

    // update height
    int height_l = get_height(node->left);
    int height_r = get_height(node->right);
    node->height = MAX(height_l, height_r) + 1;

    height_l = get_height(tmp_cell->left);
    height_r = get_height(tmp_cell->right);
    tmp_cell->height = MAX(height_l, height_r) + 1;
    return tmp_cell;
}

static position double_rotate_with_left(position node)
{
    node->left = single_rotate_with_right(node->left);
    return single_rotate_with_left(node);
}

static position double_rotate_with_right(position node)
{
    node->right = single_rotate_with_left(node->right);
    return single_rotate_with_right(node);
}

avl_tree insert(ElementType x, avl_tree tree)
{
    if (tree == NULL)
    {
        tree = malloc(sizeof(struct avl_node));
        if (tree == NULL)
        {
            printf("out of space\n");
            return NULL;
        }
        tree->element = x;
        tree->left = tree->right = NULL;
        tree->height = 0;
    }
    else if (x < tree->element)
    {
        tree->left = insert(x, tree->left);
        if (get_height(tree->left) - get_height(tree->right) == 2)
            if (x < tree->left->element)
                tree = single_rotate_with_left(tree);
            else
                tree = double_rotate_with_left(tree);
    }
    else if (x > tree->element)
    {
        tree->right = insert(x, tree->right);
        if (get_height(tree->right) - get_height(tree->left) == 2)
            if (x > tree->right->element)
                tree = single_rotate_with_right(tree);
            else
                tree = double_rotate_with_right(tree);
    }
    int height_l = get_height(tree->left);
    int height_r = get_height(tree->right);
    tree->height = MAX(height_l, height_r) + 1;
    return tree;
}

int print_tree(avl_tree tree)
{
    if (tree == NULL)
        return 0;
    else
    {
        print_tree(tree->left);
        printf("%d ", tree->element);
        print_tree(tree->right);
    }
}

int create_array(int *array, int len, int max)
{
    int temp[max];
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    // for (int i = 0; i < len; i++)
    //     array[i] = i;
    for (int i = 0; i < len; i++)
    {
        int r = rand() % max;
        // int tmp = array[i];
        // array[i] = array[r];
        // array[r] = tmp;
        temp[i] = r;
    }
    for (int i = 0; i < len; i++)
        array[i] = temp[i];

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
    srand(time(NULL));   // Initialization, should only be called once.
    int number = 5000;
    int array[number]; 
    if (create_array(array, number, number) == -1)
        return -1;
    // printf("\narray: ");
    // print_array(array, number);

    int find_num = 50;
    int find_max = 5000;
    int array_find[find_num];
    if (create_array(array_find, find_num, find_max) == -1)
        return -1;
    
    // printf("\narray_find: ");
    // print_array(array_find, find_num);

    clock_t start_t, finish_t;
    double total_t = 0;
    start_t = clock();

    for (int j = 0; j < 10000; j++)
    {
        avl_tree tree = NULL;
        for (int i = 0; i < number; i++)
            tree = insert(array[i], tree);
        // printf("\ntree: \n");
        // print_tree_structure(tree);
        avl_tree out_tree;
        for (int i = 0; i < find_num; i++)
        {
            out_tree = find(array_find[i], tree);
            // printf("\nafter find %d, the updated tree: \n", array_find[i]);
            // print_tree_structure(tree);
        }

    }
    
    finish_t = clock();
    total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;

    printf("total time: %f, rotate number: %d\n", total_t, rotate_num);
}