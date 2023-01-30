/*
note:
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int
#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct tree_node;
typedef struct tree_node *position;
typedef struct tree_node *search_tree;

struct tree_node
{
    ElementType element;
    search_tree left;
    search_tree right;
    int x;
    int y;
    int height;
};

static int get_height(position p)
{
    if (p == NULL)
        return -1;
    else
        return p->height;
}

search_tree make_empty(search_tree tree)
{
    if (tree != NULL)
    {
        make_empty(tree->left);
        make_empty(tree->right);
        tree = NULL;
    }
    return NULL;
}

position find(ElementType x, search_tree tree)
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

position find_min(search_tree tree)
{
    if (tree == NULL)
        return NULL;
    else if (tree->left != NULL)
        return find_min(tree->left);
    else
        return tree;
}

position find_max(search_tree tree)
{
    if (tree == NULL)
        return NULL;
    else if (tree->right != NULL)
        return find_max(tree->right);
    else
        return tree;
}

search_tree insert(ElementType x, search_tree tree)
{
    if (tree == NULL)
    {
        tree = malloc(sizeof(struct tree_node));
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
        tree->left = insert(x, tree->left);
    else if (x > tree->element)
        tree->right = insert(x, tree->right);
    int height_l = get_height(tree->left);
    int height_r = get_height(tree->right);
    tree->height = MAX(height_l, height_r) + 1;
    return tree;
}

int print_tree(search_tree tree)
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

int print_tree_structure(search_tree tree)
{
    if (tree == NULL)
        return 0;
    else
    {
        print_tree_structure(tree->left);
        int left_child, right_child;
        left_child = right_child = -1;
        if (tree->left)
            left_child = tree->left->element;
        if (tree->right)
            right_child = tree->right->element;
        printf("%d: left_child: %d, right_child: %d, height: %d, coordinate_x: %d, coordinate_y: %d\n", \
            tree->element, left_child, right_child, tree->height, tree->x, tree->y);
        print_tree_structure(tree->right);
    }
}

int set_coordinate(search_tree tree, int count_x, int count_y)
{
    if (tree == NULL)
        return 0;
    tree->x = count_x;
    tree->y = -1 * count_y;
    count_y++;
    if (tree->left)
        set_coordinate(tree->left, count_x - 1, count_y);
    if (tree->right)
        set_coordinate(tree->right, count_x + 1, count_y);
    return 0;
}

// int get_tree_depth(search_tree tree, int *depth)
// {
//     if (tree == NULL)
//         return -1;
//     if (*depth < (-tree->y))
//         *depth = (-tree->y);
//     if (tree->left)
//         get_tree_depth(tree->left, depth);
//     if (tree->right)
//         get_tree_depth(tree->right, depth);
//     return 0;
// }

int get_fig_width(search_tree tree)
{
    if (tree == NULL)
        return -1;
    
    int min_x = find_min(tree)->x;
    int max_x = find_max(tree)->x;

    return max_x - min_x;
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
    srand(time(NULL));   // Initialization, should only be called once.
    int number = 10;
    int array[number];
    if (create_array(array, number) == -1)
        return -1;
    
    printf("\narray: ");
    print_array(array, number);

    search_tree tree = NULL;
    for (int i = 0; i < number; i++)
        tree = insert(array[i], tree);
    int count_x = 0;
    int count_y = 0;
    set_coordinate(tree, count_x, count_y);
    printf("\ntree: ");
    print_tree_structure(tree);
    printf("\n");

    int tree_width = get_fig_width(tree);
    if (tree_width < 0)
        return -1;
    int tree_height = get_height(tree);
    float width_height_unit = 3 * (float)tree_height / (2 * (float)tree_width);
    printf("tree figure width: %d, height: %d, width_unit/height_unit: %f\n", tree_width, tree_height, width_height_unit);
}