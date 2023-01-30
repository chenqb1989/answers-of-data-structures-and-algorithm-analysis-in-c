/*
note:
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int

int recursion_count = 0;

struct tree_node;
typedef struct tree_node *position;
typedef struct tree_node *binary_tree;

struct tree_node
{
    ElementType element;
    binary_tree left;
    binary_tree right;
};

binary_tree create_tree_sample1()
{
    binary_tree tree = malloc(sizeof(struct tree_node));
    if (tree == NULL)
        return NULL;
    tree->element = 5;

    binary_tree tmp_cell1 = malloc(sizeof(struct tree_node));
    tmp_cell1->element = 2;
    tree->left = tmp_cell1;

    binary_tree tmp_cell2 = malloc(sizeof(struct tree_node));
    tmp_cell2->element = 1;
    tmp_cell1->left = tmp_cell2;

    binary_tree tmp_cell3 = malloc(sizeof(struct tree_node));
    tmp_cell3->element = 4;
    tmp_cell1->right = tmp_cell3;

    binary_tree tmp_cell4 = malloc(sizeof(struct tree_node));
    tmp_cell4->element = 3;
    tmp_cell3->left = tmp_cell4;

    binary_tree tmp_cell5 = malloc(sizeof(struct tree_node));
    tmp_cell5->element = 6;
    tree->right = tmp_cell5;

    binary_tree tmp_cell6 = malloc(sizeof(struct tree_node));
    tmp_cell6->element = 7;
    tmp_cell5->left = tmp_cell6;

    binary_tree tmp_cell7 = malloc(sizeof(struct tree_node));
    tmp_cell7->element = 8;
    tmp_cell6->left = tmp_cell7;

    return tree;
}

binary_tree create_tree_sample2()
{
    binary_tree tree = malloc(sizeof(struct tree_node));
    if (tree == NULL)
        return NULL;
    tree->element = 5;

    binary_tree tmp_cell1 = malloc(sizeof(struct tree_node));
    tmp_cell1->element = 6;
    tree->left = tmp_cell1;

    binary_tree tmp_cell2 = malloc(sizeof(struct tree_node));
    tmp_cell2->element = 7;
    tmp_cell1->left = tmp_cell2;

    binary_tree tmp_cell3 = malloc(sizeof(struct tree_node));
    tmp_cell3->element = 8;
    tmp_cell2->right = tmp_cell3;

    binary_tree tmp_cell4 = malloc(sizeof(struct tree_node));
    tmp_cell4->element = 2;
    tree->right = tmp_cell4;

    binary_tree tmp_cell5 = malloc(sizeof(struct tree_node));
    tmp_cell5->element = 4;
    tmp_cell4->left = tmp_cell5;

    binary_tree tmp_cell6 = malloc(sizeof(struct tree_node));
    tmp_cell6->element = 3;
    tmp_cell5->left = tmp_cell6;

    binary_tree tmp_cell7 = malloc(sizeof(struct tree_node));
    tmp_cell7->element = 1;
    tmp_cell4->right = tmp_cell7;

    return tree;
}

int print_tree(binary_tree tree)
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

int print_tree_structure(binary_tree tree)
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

int isomorphic(binary_tree tree1, binary_tree tree2)
{
    recursion_count++;
    if (tree1 == NULL && tree2 == NULL)
        return 1;
    int flat_isomorphic;
    if (tree1 && tree2)
    {
        position pos1 = tree1;
        position pos2 = tree2;
        if (tree1->element != tree2->element)
            return 0;
        if (!tree1->left && !tree1->right && !tree2->left && !tree2->right)
            return 1;
        else if (tree1->left && tree1->right && tree2->left && tree2->right)
        {
            if (tree1->left->element == tree2->left->element && tree1->right->element == tree2->right->element)
                return isomorphic(tree1->left, tree2->left) & isomorphic(tree1->right, tree2->right);
            else if (tree1->left->element == tree2->right->element && tree1->right->element == tree2->left->element)
                return isomorphic(tree1->left, tree2->right) & isomorphic(tree1->right, tree2->left);
        }
        else if (tree1->left && !tree1->right && tree2->left && !tree2->right)
        {
            if (tree1->left->element == tree2->left->element)
                return isomorphic(tree1->left, tree2->left);
        }
        else if (!tree1->left && tree1->right && !tree2->left && tree2->right)
        {
            if (tree1->right->element == tree2->right->element)
                return isomorphic(tree1->right, tree2->right);
        }
        else if (tree1->left && !tree1->right && !tree2->left && tree2->right)
        {
            if (tree1->left->element == tree2->right->element)
                return isomorphic(tree1->left, tree2->right);
        }
        else if (!tree1->left && tree1->right && tree2->left && !tree2->right)
        {
            if (tree1->right->element == tree2->left->element)
                return isomorphic(tree1->right, tree2->left);
        }
        else
            return 0;
    }
    return 0;
}

int main()
{
    binary_tree tree1 = create_tree_sample1();
    printf("\ntree1: ");
    print_tree_structure(tree1);
    printf("\n");

    binary_tree tree2 = create_tree_sample2();
    printf("\ntree2: ");
    print_tree_structure(tree2);
    printf("\n");
    int isom = isomorphic(tree1, tree2);
    if (isom == 1)
        printf("the two trees are isomorphic\n");
    else
        printf("the two trees are not isomorphic\n");
    printf("recusion count: %d\n", recursion_count);
}