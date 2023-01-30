/*
note:
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int rotate_num = 0;

struct splay_node;
typedef struct splay_node *position;
typedef struct splay_node *splay_tree;

struct splay_node
{
    ElementType element;
    splay_tree left;
    splay_tree right;
};

static position null_node = NULL; // needs initialization

splay_tree initialize_tree()
{
    if (null_node == NULL)
    {
        null_node = malloc(sizeof(struct splay_node));
        if (null_node == NULL)
        {
            printf("out of space\n");
            return NULL;
        }
        null_node->left = null_node->right = null_node;
    }
    return null_node;
}

static position zigzig_with_left(position node)
{
    rotate_num += 2;
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
    rotate_num += 2;
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
    rotate_num += 2;
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
    rotate_num += 2;
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
    rotate_num += 1;
    position tmp_cell = node->left;
    node->left = tmp_cell->right;
    tmp_cell->right = node;
    return tmp_cell;
}

static position single_rotate_with_right(position node)
{
    rotate_num += 1;
    position tmp_cell = node->right;
    node->right = tmp_cell->left;
    tmp_cell->left = node;
}

/* top-down splay procedure, not requiring item to be in the tree */
splay_tree splay(ElementType item, position x)
{
    static struct splay_node header;
    position left_tree_max, right_tree_min;

    header.left = header.right = null_node;
    left_tree_max = right_tree_min = &header;
    null_node->element = item;

    while (item != x->element)
    {
        if (item < x->element)
        {
            if (item < x->left->element)
                x = single_rotate_with_left(x);
            if (x->left == null_node)
                break;
            
            /* link right */
            right_tree_min->left = x;
            right_tree_min = x;
            x = x->left;
        }
        else
        {
            if (item > x->right->element)
                x = single_rotate_with_right(x);
            if (x->right == null_node)
                break;
            
            /* link left */
            left_tree_max->right = x;
            left_tree_max = x;
            x = x->right;
        } 
    }
    /* reassemble */
    left_tree_max->right = x->left;
    right_tree_min->left = x->right;
    x->left = header.right;
    x->right = header.left;

    return x;
}

splay_tree insert(ElementType item, splay_tree tree)
{
    static position new_node = NULL;
    if (new_node == NULL)
    {
        new_node = malloc(sizeof(struct splay_node));
        if (new_node == NULL)
        {
            fprintf(stderr, "out of space\n");
            return NULL;
        }
    }
    new_node->element = item;

    if (tree == null_node)
    {
        new_node->left = new_node->right = null_node;
        tree = new_node;
    }
    else
    {
        tree = splay(item, tree);
        if (item < tree->element)
        {
            new_node->left = tree->left;
            new_node->right = tree;
            tree->left = null_node;
            tree = new_node;
        }
        else if (tree->element < item)
        {
            new_node->right = tree->right;
            new_node->left = tree;
            tree->right = null_node;
            tree = null_node;
        }
        else
            return tree; // already in the tree
    }
    new_node = NULL;
    return tree;
}

splay_tree remove(ElementType item, splay_tree tree)
{
    position new_tree;
    if (tree != null_node)
    {
        tree = splay(item, tree);
        if (item == tree->element)
        {
            /* found it */
            if (tree->left == null_node)
                new_tree = tree->right;
            else
            {
                new_tree = tree->left;
                new_tree = splay(item, new_tree);
                new_tree->right = tree->right;
            }
            free(tree);
            tree = new_tree;
        }
    }
    return tree;
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
    int number = 5000;
    int array[number]; 
    if (create_array(array, number, number) == -1)
        return -1;
    // printf("\narray: ");
    // print_array(array, number);

    int find_num = 50;
    int find_max = 5000;
    int array_find[find_num];
    if (create_array(array_find, find_num, find_max) == -1)
        return -1;
    
    // printf("\narray_find: ");
    // print_array(array_find, find_num);

    clock_t start_t,finish_t;
    double total_t = 0;
    start_t = clock();

    for (int j = 0; j < 10000; j++)
    {
        splay_tree tree = NULL;
        for (int i = 0; i < number; i++)
            tree = insert(array[i], tree);
        // printf("\ntree: \n");
        // print_tree_structure(tree);
        for (int i = 0; i < find_num; i++)
        {
            tree = find(array_find[i], tree);
            // printf("\nafter find %d, the updated tree: \n", array_find[i]);
            // print_tree_structure(tree);
        }

    }
    
    finish_t = clock();
    total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;

    printf("total time: %f, rotate number: %d\n", total_t, rotate_num);
}