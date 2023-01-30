/*
note:
1. insert firstly; then from bottom up, check if node violates avl balance rule,
    add 'father' for node to do this (line: 25).
2. note the condition of violation of avl balance rule (line: 174, 206)
3. note to update father (line: 95-99)
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
    avl_tree father;
    int height;
};

static int get_height(position p)
{
    if (p == NULL)
        return -1;
    else
        return p->height;
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

    // update father
    tmp_cell->father = node->father;
    node->father = tmp_cell;
    if (node->left)
        node->left->father = node;
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

    tmp_cell->father = node->father;
    node->father = tmp_cell;
    if (node->right)
        node->right->father = node;
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
    position tmp_cell = malloc(sizeof(struct avl_node));
    tmp_cell->element = x;
    tmp_cell->left = tmp_cell->right = NULL;
    tmp_cell->height = 0;

    if (tree == NULL)
    {
        tree = tmp_cell;
        tree->father = NULL;
    }
    else
    {
        position node = tree;
        position father_node = NULL;
        while (node != NULL)
        {
            father_node = node;
            if (x < node->element)
                node = node->left;
            else if (x > node->element)
                node = node->right;
        }
        tmp_cell->father = father_node;
        if (father_node->element > tmp_cell->element)
            father_node->left = tmp_cell;
        else if (father_node->element < tmp_cell->element)
            father_node->right = tmp_cell;

        while (father_node != NULL)
        {
            int height_l = get_height(father_node->left);
            int height_r = get_height(father_node->right);
            father_node->height = MAX(height_l, height_r) + 1;
            if (height_l - height_r == 2)
            {
                height_l = get_height(father_node->left->left);
                height_r = get_height(father_node->left->right);
                if (height_l > height_r)
                {
                    position father_father_node = father_node->father;
                    father_node = single_rotate_with_left(father_node);
                    if (father_father_node)
                        // father_node is not the root
                        if (father_father_node->element > father_node->element)
                            father_father_node->left = father_node;
                        else
                            father_father_node->right = father_node;
                    else
                        // father_node is the root
                        tree = father_node;
                }
                    
                else if (height_l < height_r)
                {
                    position father_father_node = father_node->father;
                    father_node = double_rotate_with_left(father_node);
                    if (father_father_node)
                        if (father_father_node->element > father_node->element)
                            father_father_node->left = father_node;
                        else
                            father_father_node->right = father_node;
                    else
                        tree = father_node;
                }    
                break;
            }
            else if (height_r - height_l == 2)
            {
                height_l = get_height(father_node->right->left);
                height_r = get_height(father_node->right->right);
                if (height_l < height_r)
                {
                    position father_father_node = father_node->father;
                    father_node = single_rotate_with_right(father_node);
                    if (father_father_node)
                        if (father_father_node->element > father_node->element)
                            father_father_node->left = father_node;
                        else
                            father_father_node->right = father_node;
                    else
                        tree = father_node;
                }
                    
                else if (height_l > height_r)
                {
                    position father_father_node = father_node->father;
                    father_node = double_rotate_with_right(father_node);
                    if (father_father_node)
                        if (father_father_node->element > father_node->element)
                            father_father_node->left = father_node;
                        else
                            father_father_node->right = father_node;
                    else
                        tree = father_node;
                }
                break;
            }
            father_node = father_node->father;
        }
        if (father_node != NULL)
        {
            int height_l = get_height(father_node->left);
            int height_r = get_height(father_node->right);
            father_node->height = MAX(height_l, height_r) + 1;
        }
        
    }
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

int print_tree_structure(avl_tree tree)
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
        int r = rand() % 15;
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
    int number = 15;
    int array[15]; 
    if (create_array(array, number) == -1)
        return -1;
    
    printf("\narray: ");
    print_array(array, number);

    avl_tree tree = NULL;
    for (int i = 0; i < number; i++)
        tree = insert(array[i], tree);
    printf("\ntree: ");
    print_tree_structure(tree);
    int tree_height = get_height(tree);
    printf("\ntree height: %d\n", tree_height);
    return 0;
}