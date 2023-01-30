/*
note:
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int

struct tree_node;
typedef struct tree_node *position;
typedef struct tree_node *child_sibling_tree;

struct tree_node
{
    ElementType element;
    child_sibling_tree first_child;
    child_sibling_tree next_sibling;
};

child_sibling_tree create_sample_tree()
{
    child_sibling_tree tree = malloc(sizeof(struct tree_node));
    if (tree == NULL)
        return NULL;
    tree->element = 100;

    position pos = tree;

    for (int i = 0; i < 20; i++)
    {
        position tmp_cell = malloc(sizeof(struct tree_node));
        if (tmp_cell == NULL)
            return NULL;
        tmp_cell->element = i;
        // int r = rand() % 20;
        if (i % 2 == 0)
            pos->first_child = tmp_cell;
        else
            pos->next_sibling = tmp_cell;
        pos = tmp_cell;
    }
    return tree;
}

int print_tree(child_sibling_tree tree)
{
    if (tree == NULL)
        return 0;
    else
    {
        if (tree->first_child != NULL)
            print_tree(tree->first_child);
        printf("%d ", tree->element);
        if (tree->next_sibling != NULL)
            print_tree(tree->next_sibling);
    }
}

int main()
{
    child_sibling_tree tree = create_sample_tree();
    print_tree(tree);
    printf("\n");
}