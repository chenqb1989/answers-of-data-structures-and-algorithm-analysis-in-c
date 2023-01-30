/*
note:
1. use queue
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int
#define MinQueueSize (5)
#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct queue_record;
typedef struct queue_record *t_queue;

struct tree_node;
typedef struct tree_node *position;
typedef struct tree_node *search_tree;

struct queue_record
{
    int capacity;
    int front;
    int rear;
    int size;
    search_tree *data;
};

struct tree_node
{
    ElementType element;
    search_tree left;
    search_tree right;
    int x;
    int y;
    int height;
};

int is_empty(t_queue que)
{
    return que->size == 0;
}

t_queue create_queue(int max_elements)
{
    if (max_elements < MinQueueSize)
    {
        printf("queue size is too small!\n");
        return NULL;
    }
    t_queue que = malloc(sizeof(struct queue_record));
    if (que == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    que->data = malloc(sizeof(struct tree_node) * max_elements);
    if (que->data == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    que->front = 1;
    que->rear = 0;
    que->size = 0;
    que->capacity = max_elements;
    return que;
}

static int succ(int value, t_queue que)
{
    if (++value == que->capacity)
        value = 0;
    return value;
}

int enqueue(t_queue que, search_tree x)
{
    if (que->size == que->capacity)
    {
        printf("queue is full!\n");
        return -1;
    }
    que->rear = succ(que->rear, que);
    que->data[que->rear] = x;
    que->size++;
    return 0;
}

search_tree dequeue(t_queue que)
{
    if (is_empty(que))
    {
        printf("queue is empty!\n");
        return NULL;
    }
    search_tree result = que->data[que->front];
    que->front = succ(que->front, que);
    que->size--;
    return result;
}

int free_queue(t_queue que)
{
    if (que == NULL)
        return 0;
    free(que);
}

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

int free_tree(search_tree tree)
{
    if (tree != NULL)
    {
        free_tree(tree->left);
        free_tree(tree->right);
        free(tree);
    }
    return 0;
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
        tree->height = 0;
    }
    else if (x < tree->element)
        tree->left = insert(x, tree->left);
    else if (x > tree->element)
        tree->right = insert(x, tree->right);
    int height_l = get_height(tree->left);
    int height_r = get_height(tree->right);
    tree->height = MAX(height_l, height_r) + 1;
    return tree;
}

int level_print(search_tree tree)
{
    if (tree == NULL)
        return 0;
    t_queue queue = create_queue(10);
    enqueue(queue, tree);
    while(queue->size > 0)
    {
        position node = dequeue(queue);
        printf("%d ", node->element);
        if (node->left)
            enqueue(queue, node->left);
        if (node->right)
            enqueue(queue, node->right);
    }
    free(queue);
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
        printf("%d: left_child: %d, right_child: %d, height: %d\n", \
            tree->element, left_child, right_child, tree->height);
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
    int number = 10;
    int array[number];
    if (create_array(array, number) == -1)
        return -1;
    
    printf("\narray: ");
    print_array(array, number);

    search_tree tree = NULL;
    for (int i = 0; i < number; i++)
        tree = insert(array[i], tree);
    printf("\ntree: ");
    print_tree_structure(tree);
    printf("\n");
    printf("\nlevel print tree: ");
    level_print(tree);
    printf("\n");
    free_tree(tree);
}