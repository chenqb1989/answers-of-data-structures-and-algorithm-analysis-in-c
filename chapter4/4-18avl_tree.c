/*
note:
1. note the roated nodes, eg. single rotate with left, roate node and its left child
2. double rotate is composed of two single rotates(line: 122-123)
3. note the order: rotate after inserting(line: 149-154)
4. MAX should use parentheses
5. note to update height
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int
#define MAX(a, b) ((a) > (b) ? (a) : (b))

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
        int r = rand() % 10;
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
    srand(time(NULL));   // Initialization, should only be called once.
    int number = 10;
    int array[10]; 
    if (create_array(array, number) == -1)
        return -1;
    
    printf("\narray: ");
    print_array(array, number);

    avl_tree tree = NULL;
    for (int i = 0; i < number; i++)
        tree = insert(array[i], tree);
    printf("\ntree: ");
    print_tree(tree);
    int tree_height = get_height(tree);
    printf("\ntree height: %d\n", tree_height);
}