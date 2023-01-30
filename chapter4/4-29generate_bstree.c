/*
note:
1. most time might be spent on 'create_array'
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int

struct tree_node;
typedef struct tree_node *position;
typedef struct tree_node *search_tree;

struct tree_node
{
    ElementType element;
    search_tree left;
    search_tree right;
};

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
    }
    else if (x < tree->element)
        tree->left = insert(x, tree->left);
    else if (x > tree->element)
        tree->right = insert(x, tree->right);
    return tree;
}

search_tree delete(ElementType x, search_tree tree)
{
    position tmp_cell;
    if (tree == NULL)
    {
        printf("element not found\n");
        return NULL;
    }
    else if (x < tree->element)
        tree->left = delete(x, tree->left);
    else if (x > tree->element)
        tree->right = delete(x, tree->right);
    else if (tree->left && tree->right)
    {
        tmp_cell = find_min(tree->right);
        tree->element = tmp_cell->element;
        tree->right = delete(tree->element, tree->right);
    }
    else
    {
        tmp_cell = tree;
        if (tree->left == NULL)
            tree = tree->right;
        else if (tree->right == NULL)
            tree = tree->left;
        free(tmp_cell);
    }
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
        printf("%d: left_child: %d, right_child: %d\n", tree->element, left_child, right_child);
        print_tree_structure(tree->right);
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
    int number = 1000;
    int array[number]; 
    if (create_array(array, number) == -1)
        return -1;
    
    printf("\narray: ");
    print_array(array, number);

    search_tree tree = NULL;
    for (int i = 0; i < number; i++)
        tree = insert(array[i], tree);
    printf("\ntree: \n");
    print_tree_structure(tree);
}