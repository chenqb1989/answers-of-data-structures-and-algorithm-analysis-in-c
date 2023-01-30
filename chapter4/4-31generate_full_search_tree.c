/*
note:
1. note the order of generated node element, 
    if height = 2, the numbers: 4 2 1 3 6 5 7; if height = 3, the numbers: 8 6 2 1 3 4 5 7 12 10 9 11 14 13 15
    we use this order because we can insert them one by one without rotating, so it is efficient
2. we delete rotate codes in 'insert' function
3. note the recursion function of 'create_full_tree', it need to run two branches, which is different with 'binary search'
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define ElementType int
#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct search_node;
typedef struct search_node *position;
typedef struct search_node *search_tree;

struct search_node
{
    ElementType element;
    search_tree left;
    search_tree right;
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
        tree = malloc(sizeof(struct search_node));
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
    }
    else if (x > tree->element)
    {
        tree->right = insert(x, tree->right);
    }
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
        printf("%d: left_child: %d, right_child: %d, height: %d\n", tree->element, left_child, right_child, tree->height);
        print_tree_structure(tree->right);
    }
}

search_tree create_full_tree(search_tree tree, int min, int max, int round)
{
    if (min >= max)
        return NULL;
    int mid = (min + max) / 2;
    int remainder = (min + max) % 2;
    if (remainder == 1 && round == 1)
        mid += 1;
    
    tree = insert(mid, tree);
    if (max - min == 1 || (max - min == 2 && max % 2 == 0) )
        return NULL;
    create_full_tree(tree, min, mid, 0);
    create_full_tree(tree, mid, max, 1);
    return tree;
}

int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    int height = 6;
    int number = pow(2, height + 1) - 1;

    search_tree tree = NULL;
    tree = create_full_tree(tree, 1, number, 0);
    printf("\ntree: \n");
    print_tree_structure(tree);
    int tree_height = get_height(tree);
    printf("\ntree height: %d\n", tree_height);
}