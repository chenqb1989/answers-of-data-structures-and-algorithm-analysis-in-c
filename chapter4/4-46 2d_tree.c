/*
note:
1. check if using key1 or key2 based on depth
2. note to update depth after inserting
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int

struct tree_node;
typedef struct tree_node *position;
typedef struct tree_node *search_tree;
struct element_entry;
typedef struct element_entry *ptr_element;

struct tree_node
{
    ptr_element element;
    search_tree left;
    search_tree right;
    int depth;
};

struct element_entry
{
    ElementType key1;
    ElementType key2;
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

search_tree insert(ptr_element x, search_tree tree)
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
        tree->depth = 0;
    }
    if (tree->depth % 2 == 0)
    {
        if (x->key1 < tree->element->key1)
        {
            tree->left = insert(x, tree->left);
            tree->left->depth = tree->depth + 1;
        }
            
        else if (x->key1 > tree->element->key1)
        {
            tree->right = insert(x, tree->right);
            tree->right->depth = tree->depth + 1;
        }
            
    }
    else
    {
        if (x->key2 < tree->element->key2)
        {
            tree->left = insert(x, tree->left);
            tree->left->depth = tree->depth + 1;
        }
        else if (x->key2 > tree->element->key2)
        {
            tree->right = insert(x, tree->right);
            tree->right->depth = tree->depth + 1;
        }
    }
    return tree;
}

int print_elements(search_tree tree, ptr_element k1, ptr_element k2)
{
    if (tree == NULL)
        return -1;
    if (tree->depth % 2 == 0)
    {
        if (tree->element->key1 < k1->key1)
            print_elements(tree->right, k1, k2);
        else if (tree->element->key1 > k1->key2)
            print_elements(tree->left, k1, k2);
        else
        {
            print_elements(tree->left, k1, k2);
            if (tree->element->key2 >= k2->key1 && tree->element->key2 <= k2->key2)
                printf("key1: %d key2: %d\n", tree->element->key1, tree->element->key2);
            print_elements(tree->right, k1, k2);
        }
    }
    else
    {
        if (tree->element->key2 < k2->key1)
            print_elements(tree->right, k1, k2);
        else if (tree->element->key2 > k2->key2)
            print_elements(tree->left, k1, k2);
        else
        {
            print_elements(tree->left, k1, k2);
            if (tree->element->key1 >= k1->key1 && tree->element->key1 <= k1->key2)
                printf("key1: %d key2: %d\n", tree->element->key1, tree->element->key2);
            print_elements(tree->right, k1, k2);
        }
    }
    
    return 0;
}


int print_tree_structure(search_tree tree)
{
    if (tree == NULL)
        return 0;
    else
    {
        print_tree_structure(tree->left);
        struct element_entry left_child, right_child;
        left_child.key1 = left_child.key2 = right_child.key1 = right_child.key2 = -1;
        if (tree->left)
            left_child = *(tree->left->element);
        if (tree->right)
            right_child = *(tree->right->element);
        printf("tree->key1: %d tree->key2: %d : left_child-key1: %d, left_child-key12: %d right_child-key1: %d right_child-key1: %d\n", 
            tree->element->key1, tree->element->key2, left_child.key1, left_child.key2, right_child.key1, right_child.key2);
        print_tree_structure(tree->right);
    }
}

int create_array(ptr_element array, int len, int *arr1, int *arr2)
{
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    // ElementType arr1[len];
    // ElementType arr2[len];
    // for (int i = 0; i < len; i++)
    // {
    //     arr1[i] = i;
    //     arr2[i] = i;
    // }
        
    // for (int i = 0; i < len; i++)
    // {
    //     int r1 = rand() % 10;
    //     int tmp = arr1[i];
    //     arr1[i] = arr1[r1];
    //     arr1[r1] = tmp;

    //     int r2 = rand() % 10;
    //     tmp = arr2[i];
    //     arr2[i] = arr2[r2];
    //     arr2[r2] = tmp;
    // }
    for (int i = 0; i < len; i++)
    {
        array[i].key1 = arr1[i];
        array[i].key2 = arr2[i];
    }
    return 0;
}

int print_array(ptr_element array, int len)
{
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    printf("\nkey1:\n");
    for (int i = 0; i < len; i++)
        printf("%d ", array[i].key1);
    printf("\nkey2:\n");
    for (int i = 0; i < len; i++)
        printf("%d ", array[i].key2);
    printf("\n");
    return 0;
}

int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    int number = 10;
    struct element_entry *array = malloc(sizeof(struct element_entry) * number);
    int arr1[10] = {4, 2, 5, 8, 9, 0, 6, 7, 1, 3};
    int arr2[10] = {6, 2, 7, 1, 4, 0, 9, 8, 5, 3};
    if (create_array(array, number, arr1, arr2) == -1)
        return -1;
    
    printf("\narray: ");
    print_array(array, number);

    search_tree tree = NULL;
    for (int i = 0; i < number; i++)
        tree = insert(&array[i], tree);
    printf("\ntree: ");
    print_tree_structure(tree);
    printf("\n");

    struct element_entry k1 = {
        .key1 = 1,
        .key2 = 9,
    };
    struct element_entry k2 = {
        .key1 = 1,
        .key2 = 9,
    };
    print_elements(tree, &k1, &k2);
}