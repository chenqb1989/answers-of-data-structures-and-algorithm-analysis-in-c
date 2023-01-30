/*
note:
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int
#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct splay_node;
typedef struct splay_node *position;
typedef struct splay_node *splay_tree;

struct splay_node
{
    ElementType element;
    splay_tree left;
    splay_tree right;
};

splay_tree make_empty(splay_tree tree)
{
    if (tree != NULL)
    {
        make_empty(tree->left);
        make_empty(tree->right);
        tree = NULL;
    }
    return NULL;
}

static position zigzig_with_left(position node)
{
    position child = node->left;
    position grandson = child->left;

    node->left = child->right;
    child->left = grandson->right;

    child->right = node;
    grandson->right = child;
    return grandson;
}

static position zigzig_with_right(position node)
{
    position child = node->right;
    position grandson = child->right;

    node->right = child->left;
    child->right = grandson->left;

    child->left = node;
    grandson->left = child;
    return grandson;
}

// zigzag_with_left is same with double_rotate_with_left
static position zigzag_with_left(position node)
{
    position child = node->left;
    position grandson = child->right;

    node->left = grandson->right;
    child->right = grandson->left;
    grandson->left = child;
    grandson->right = node;
    return grandson;
}

// zigzag_with_right is same with double_rotate_with_right
static position zigzag_with_right(position node)
{
    position child = node->right;
    position grandson = child->left;

    node->right = grandson->left;
    child->left = grandson->right;
    grandson->left = node;
    grandson->right = child;
    return grandson;
}

static position single_rotate_with_left(position node)
{
    position tmp_cell = node->left;
    node->left = tmp_cell->right;
    tmp_cell->right = node;
    return tmp_cell;
}

static position single_rotate_with_right(position node)
{
    position tmp_cell = node->right;
    node->right = tmp_cell->left;
    tmp_cell->left = node;
}

position find_in(ElementType x, splay_tree tree)
{
    if (tree == NULL)
        return NULL;
    else if (x < tree->element)
    {
        tree->left = find_in(x, tree->left);
        if (tree->left)
        {
            if (tree->left->left && tree->left->left->element == x)
                return zigzig_with_left(tree);
            else if (tree->left->right && tree->left->right->element == x)
                return zigzag_with_left(tree);
            else
                return tree;
        }
        else
            return tree;
    }
    else if (x > tree->element)
    {
        tree->right = find_in(x, tree->right);
        if (tree->right)
        {
            if (tree->right->right && tree->right->right->element == x)
                return zigzig_with_right(tree);
            else if (tree->right->left && tree->right->left->element == x)
                return zigzag_with_right(tree);
            else
                return tree;
        }
        else
            return tree;
    }
    else
        return tree;
}

position find(ElementType x, splay_tree tree)
{
    if (tree == NULL)
        return NULL;
    tree = find_in(x, tree);
    if (x != tree->element)
    {
        if (tree->left && tree->left->element == x)
            tree = single_rotate_with_left(tree);
        else if (tree->right && tree->right->element == x)
            tree = single_rotate_with_right(tree);
    }
    return tree;
}

position find_min(splay_tree tree)
{
    if (tree == NULL)
        return NULL;
    else if (tree->left != NULL)
        return find_min(tree->left);
    else
        return tree;
}

position find_max(splay_tree tree)
{
    if (tree == NULL)
        return NULL;
    else if (tree->right != NULL)
        return find_max(tree->right);
    else
        return tree;
}

splay_tree insert(ElementType x, splay_tree tree)
{
    if (tree == NULL)
    {
        tree = malloc(sizeof(struct splay_node));
        if (tree == NULL)
        {
            printf("out of space\n");
            return NULL;
        }
        tree->element = x;
        tree->left = tree->right = NULL;
    }
    else if (x < tree->element)
    {
        tree->left = insert(x, tree->left);
    }
    else if (x > tree->element)
    {
        tree->right = insert(x, tree->right);
    }
    return tree;
}

int print_tree(splay_tree tree)
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

int print_tree_structure(splay_tree tree)
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

int count_tree(splay_tree tree, int *node_num, int *leaf_num, int *full_num)
{
    if (tree == NULL)
        return -1;
    if (tree->left)
        count_tree(tree->left, node_num, leaf_num, full_num);
    
    (*node_num)++;
    if (tree->left && tree->right)
        (*full_num)++;
    else if (tree->left == NULL && tree->right == NULL)
        (*leaf_num)++;
    
    if (tree->right)
        count_tree(tree->right, node_num, leaf_num, full_num);
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
    int number = 10;
    int array[number]; 
    if (create_array(array, number, number) == -1)
        return -1;
    printf("\narray: ");
    print_array(array, number);

    splay_tree tree = NULL;
    for (int i = 0; i < number; i++)
        tree = insert(array[i], tree);
    printf("\ntree: \n");
    print_tree_structure(tree);

    int node_num = 0;
    int leaf_num = 0;
    int full_num = 0;
    if (count_tree(tree, &node_num, &leaf_num, &full_num) < 0)
        return -1;
    printf("\ntotal node number: %d, leaf node number: %d, full node number: %d\n", node_num, leaf_num, full_num);

}