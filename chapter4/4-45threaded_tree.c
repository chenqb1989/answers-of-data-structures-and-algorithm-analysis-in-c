/*
note:
1. add left_null and right_null to indicate child is null or not
2. in 'insert', need to check child is null or not firstly and deal with null independently(line: 85-101)
3. in 'delete', need to update left_null and right_null once after deleting (line: 139-147). 
    when both children are null, then return null, and deal with it independently
4. I tried to not add any attribute, though i success in 'insert', 
    but failed in 'delete', any other way to implement 'delete'?
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int

struct tree_node;
typedef struct tree_node *position;
typedef struct tree_node *threaded_tree;

struct tree_node
{
    ElementType element;
    threaded_tree left;
    threaded_tree right;
    char left_null;
    char right_null;
};

threaded_tree make_empty(threaded_tree tree)
{
    if (tree != NULL)
    {
        make_empty(tree->left);
        make_empty(tree->right);
        tree = NULL;
    }
    return NULL;
}

position find_min(threaded_tree tree)
{
    if (tree == NULL)
        return NULL;
    else if (!tree->left_null)
        return find_min(tree->left);
    else
        return tree;
}

int print_tree_structure(threaded_tree tree)
{
    if (tree == NULL)
        return 0;
    else
    {
        if (!tree->left_null)
            print_tree_structure(tree->left);
        int left_child, right_child;
        left_child = right_child = -1;
        if (!tree->left_null)
            left_child = tree->left->element;
        if (!tree->right_null)
            right_child = tree->right->element;
        printf("%d: left_child: %d, right_child: %d\n", tree->element, left_child, right_child);
        if (!tree->right_null)
            print_tree_structure(tree->right);
    }
}

threaded_tree insert(ElementType x, threaded_tree tree)
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
        tree->left = tree->right = tree;
        tree->left_null = 1;
        tree->right_null = 1;
    }
    else if (x < tree->element)
    {
        if (tree->left_null)
        {
            position tmp_cell = malloc(sizeof(struct tree_node));
            if (tmp_cell == NULL)
            {
                printf("out of space\n");
                return NULL;
            }
            tmp_cell->element = x;
            tree->left = tmp_cell;
            tree->left_null = 0;

            tmp_cell->left = tmp_cell->right = tree;
            tmp_cell->left_null = tmp_cell->right_null = 1;
        }
        else
            tree->left = insert(x, tree->left);
            
    }  
    else if (x > tree->element)
    {
        if (tree->right_null)
        {
            position tmp_cell = malloc(sizeof(struct tree_node));
            if (tmp_cell == NULL)
            {
                printf("out of space\n");
                return NULL;
            }
            tmp_cell->element = x;
            tree->right = tmp_cell;
            tree->right_null = 0;

            tmp_cell->left = tmp_cell->right = tree;
            tmp_cell->left_null = tmp_cell->right_null = 1;
        }
        else
            tree->right = insert(x, tree->right);
    }
    return tree;
}

threaded_tree delete(ElementType x, threaded_tree tree)
{
    position tmp_cell;
    if (tree == NULL)
    {
        printf("element %d not found\n", x);
        return NULL;
    }
    else if (x < tree->element)
    {
        tree->left = delete(x, tree->left);
        if (tree->left == NULL)
            tree->left_null = 1;
        else
        {
            if (tree->left->left_null)
                tree->left->left = tree;
            if (tree->left->right_null)
                tree->left->right = tree;
        }
    }
    else if (x > tree->element)
    {
        tree->right = delete(x, tree->right);
        if (tree->right == NULL)
            tree->right_null = 1;
        else
        {
            if (tree->right->left_null)
                tree->right->left = tree;
            if (tree->right->right_null)
                tree->right->right = tree;
        }
    }
    else if (!tree->left_null && !tree->right_null)
    {
        tmp_cell = find_min(tree->right);
        tree->element = tmp_cell->element;
        tree->right = delete(tree->element, tree->right);
        if (tree->right == NULL)
            tree->right_null = 1;
        else
        {
            if (tree->right->left_null)
                tree->right->left = tree;
            if (tree->right->right_null)
                tree->right->right = tree;
        }
    }
    else
    {
        tmp_cell = tree;
        if (tree->left_null && tree->right_null)
            tree = NULL;
        else if (tree->left_null)
            tree = tree->right;
        else if (tree->right_null)
            tree = tree->left;
        free(tmp_cell);
    }
    return tree;
}

// // position find(ElementType x, threaded_tree tree)
// // {
// //     if (tree == NULL)
// //         return NULL;
// //     else if (x < tree->element)
// //         return find(x, tree->left);
// //     else if (x > tree->element)
// //         return find(x, tree->right);
// //     else
// //         return tree;
// // }

// position find_min(threaded_tree tree)
// {
//     if (tree == NULL)
//         return NULL;
//     else if (tree->left->element < tree->element)
//         return find_min(tree->left);
//     else
//         return tree;
// }

// threaded_tree insert(ElementType x, threaded_tree tree)
// {
//     if (tree == NULL)
//     {
//         tree = malloc(sizeof(struct tree_node));
//         if (tree == NULL)
//         {
//             printf("out of space\n");
//             return NULL;
//         }
//         tree->element = x;
//         tree->left = tree->right = tree;
//         tree->left_null = 1;
//         tree->right_null = 1;
//     }
//     else if (x < tree->element)
//     {
//         if (tree->left == tree)
//         {
//             // tree node is the root, and it has no left child
//             position tmp_cell = malloc(sizeof(struct tree_node));
//             if (tmp_cell == NULL)
//             {
//                 printf("out of space\n");
//                 return NULL;
//             }
//             tmp_cell->element = x;
//             tree->left = tmp_cell;
//             tmp_cell->left = tmp_cell->right = tree;
//         }
//         else if (x < tree->left->element)
//         {
//             if (tree->left->left == tree)
//             {
//                 // tree left left is null
//                 position tmp_cell = malloc(sizeof(struct tree_node));
//                 if (tmp_cell == NULL)
//                 {
//                     printf("out of space\n");
//                     return NULL;
//                 }
//                 tmp_cell->element = x;

//                 tree->left->left = tmp_cell;
//                 tmp_cell->left = tmp_cell->right = tree->left;
//             }
//             else
//                 tree->left = insert(x, tree->left);
//         }
//         else if (x > tree->left->element)
//         {
//             if (tree->left->right == tree)
//             {
//                 // tree left right is null
//                 position tmp_cell = malloc(sizeof(struct tree_node));
//                 if (tmp_cell == NULL)
//                 {
//                     printf("out of space\n");
//                     return NULL;
//                 }
//                 tmp_cell->element = x;

//                 tree->left->right = tmp_cell;
//                 tmp_cell->left = tmp_cell->right = tree->left;
//             }
//             else
//                 tree->left = insert(x, tree->left);
//         }
            
//     }  
//     else if (x > tree->element)
//     {
//         if (tree->right == tree)
//         {
//             // tree node is the root, and it has no right child
//             position tmp_cell = malloc(sizeof(struct tree_node));
//             if (tmp_cell == NULL)
//             {
//                 printf("out of space\n");
//                 return NULL;
//             }
//             tmp_cell->element = x;
//             tree->right = tmp_cell;
//             tmp_cell->left = tmp_cell->right = tree;

//         }
//         else if (x > tree->right->element)
//         {
//             if (tree->right->right == tree)
//             {
//                 // tree right right is null
//                 position tmp_cell = malloc(sizeof(struct tree_node));
//                 if (tmp_cell == NULL)
//                 {
//                     printf("out of space\n");
//                     return NULL;
//                 }
//                 tmp_cell->element = x;

//                 tree->right->right = tmp_cell;
//                 tmp_cell->left = tmp_cell->right = tree->right;
//             }
//             else
//                 tree->right = insert(x, tree->right);
//         }
//         else if (x < tree->right->element)
//         {
//             if (tree->right->left == tree)
//             {
//                 // tree right right is null
//                 position tmp_cell = malloc(sizeof(struct tree_node));
//                 if (tmp_cell == NULL)
//                 {
//                     printf("out of space\n");
//                     return NULL;
//                 }
//                 tmp_cell->element = x;

//                 tree->right->left = tmp_cell;
//                 tmp_cell->left = tmp_cell->right = tree->right;
//             }
//             else
//                 tree->right = insert(x, tree->right);
//         }
//     }
//     return tree;
// }

// threaded_tree delete(ElementType x, threaded_tree tree, int direction)
// {
//     position tmp_cell;
//     if (tree == NULL)
//     {
//         printf("element not found\n");
//         return NULL;
//     }
//     else if (x < tree->element)
//     {
//         tree->left = delete(x, tree->left, 1);
//         if (tree->left->left == NULL)  // tree->left->left is null
//             tree->left->left = tree;
//         if (tree->left->right == NULL)
//             tree->left->right = tree;
//     }
//     else if (x > tree->element)
//     {
//         tree->right = delete(x, tree->right, 2);
//         if (tree->right->left == NULL)  // tree->right->left is null
//             tree->right->left = tree;
//         if (tree->right->right == NULL)
//             tree->right->right = tree;
//     }
//     else if ((direction == 1 && tree->left->left != tree && tree->right->left != tree) || \
//             (direction == 2 && tree->left->right != tree && tree->right->right != tree)|| \
//             (direction == 0 && tree->left != tree && tree->right != tree))
//     {
//         // both children of tree are not null
//         tmp_cell = find_min(tree->right);
//         tree->element = tmp_cell->element;
//         tree->right = delete(tree->element, tree->right, 2);
//         if (tree->right->left == NULL)  // tree->right->left is null
//             tree->right->left = tree;
//         if (tree->right->right == NULL)
//             tree->right->right = tree;
//     }
//     else
//     {
//         tmp_cell = tree;
//         if ((direction == 1 && tree->left->left == tree && tree->right->left != tree) || \
//             (direction == 1 && tree->left->left == tree && tree->right->left == tree && tree->left != tree->right) ||
//             (direction == 2 && tree->left->right == tree && tree->right->right != tree) ||\
//             (direction == 2 && tree->left->right == tree && tree->right->right == tree && tree->left != tree->right) ||
//             (direction == 0 && tree->left == tree && tree->right != tree))
//         {
//             // left child is null
//             if (tree->right->left == tree)
//                 tree->right->left = NULL;
//             if (tree->right->right == tree)
//                 tree->right->right = NULL;
            
//             tree = tree->right;
//         }
//         else if ((direction == 1 && tree->left->left != tree && tree->right->left == tree) || 
//                 (direction == 1 && tree->left->left == tree && tree->right->left == tree && tree->left != tree->right) ||
//             (direction == 2 && tree->left->right != tree && tree->right->right == tree) || \
//             (direction == 2 && tree->left->right == tree && tree->right->right == tree && tree->left != tree->right) ||
//             (direction == 0 && tree->right == tree && tree->left != tree))
//         {
//             // right child is null
//             if (tree->left->left == tree)
//                 tree->left->left = NULL;
//             if (tree->left->right == tree)
//                 tree->left->right = NULL;
//             tree = tree->left;
//         }
//         else
//         {
//             // both children of tree are null
//             tree = NULL;
//         }
//         free(tmp_cell);
//     }
//     return tree;
// }

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
    int array1[number]; 
    if (create_array(array1, number) == -1)
        return -1;
    // int array[10] = {2, 1, 5, 6, 0, 3, 4, 7, 9, 8};
    
    printf("\narray1: ");
    print_array(array1, number);

    threaded_tree tree = NULL;
    for (int i = 0; i < number; i++)
        tree = insert(array1[i], tree);
    
    printf("\ntree: ");
    print_tree_structure(tree);

    int array2[number]; 
    if (create_array(array2, number) == -1)
        return -1;
    printf("\narray2: ");
    print_array(array2, number);

    for (int i = 0; i < number; i++)
    {
        tree = delete(array2[i], tree);
        printf("\n\nafter deleting %d, the tree: \n", array2[i]);
        print_tree_structure(tree);
    }
    return 0;
}