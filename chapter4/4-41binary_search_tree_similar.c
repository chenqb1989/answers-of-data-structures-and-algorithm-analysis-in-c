/*
note:
1. two tree are similar when they have same sturcture, rather than data
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

int compare_search_tree(search_tree tree1, search_tree tree2)
{
    if (tree1 == NULL && tree2 == NULL)
        return 1;
    
    if (tree1 && tree2)
    {
        position pos1 = tree1;
        position pos2 = tree2;
        if ((pos1->left && pos2->left == NULL) || (pos1->left == NULL && pos2->left)\
            || (pos1->right && pos2->right == NULL) || (pos1->right == NULL && pos2->right))
            return 0;
        
        int flag_similar;
        if (pos1->left)
        {
            flag_similar = compare_search_tree(pos1->left, pos2->left);
            if (flag_similar == 0)
                return flag_similar;
        }
            
        if (pos1->right)
        {
            flag_similar = compare_search_tree(pos1->right, pos2->right);
            if (flag_similar == 0)
                return flag_similar;
        }
        return 1;
    }
    else
        return 0;
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
    int number = 4;
    int array[number]; 
    if (create_array(array, number) == -1)
        return -1;
    
    printf("\narray: ");
    print_array(array, number);

    search_tree tree1 = NULL;
    for (int i = 0; i < number; i++)
        tree1 = insert(array[i], tree1);
    printf("\ntree1: ");
    print_tree_structure(tree1);
    printf("\n");

    int array2[number]; 
    if (create_array(array2, number) == -1)
        return -1;
    
    printf("\narray: ");
    print_array(array2, number);

    search_tree tree2 = NULL;
    for (int i = 0; i < number; i++)
        tree2 = insert(array2[i], tree2);
    printf("\ntree2: ");
    print_tree_structure(tree2);
    printf("\n");
    int similar = compare_search_tree(tree1, tree2);
    if (similar == 1)
        printf("the two trees are similar\n");
    else
        printf("the two trees are not similar\n");
}