/*
note:
1. when need to use the min or max, we can consider priority heap
2. in this situation, we need to find the box with max capacity, 
    and i convert the question to find the box with min used capacity, to use min_heap
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define ElementType int
#define MinPQSize 1
#define MinData -1
#define MaxElementSize 100
#define C 5

struct heap_struct;
typedef struct heap_struct *priority_query;

struct heap_struct
{
    int capacity;
    int size;
    ElementType *elements;
};

priority_query initialize(int max_elments)
{
    priority_query h;

    if (max_elments < MinPQSize)
    {
        printf("priority queue size is too small\n");
        return NULL;
    }

    h = malloc(sizeof(struct heap_struct));
    if (h == NULL)
    {
        printf("out of space\n");
        return NULL;
    }

    h->elements = malloc(sizeof(ElementType) * (max_elments + 1));
    if (h->elements == NULL)
    {
        printf("out of space\n");
        return NULL;
    }

    h->capacity = max_elments;
    h->size = 0;
    h->elements[0] = MinData;
    return h;
}

int is_full(priority_query h)
{
    if (h->size == h->capacity)
        return 1;
    else
        return 0;
}

int is_empty(priority_query h)
{
    if (h->size == 0)
        return 1;
    else
        return 0;
}

int insert(ElementType x, priority_query h)
{
    if (is_full(h))
    {
        printf("the priority queue is full\n");
        return -1;
    }

    int i = ++h->size;
    for (; h->elements[i / 2] > x; i /= 2)
        h->elements[i] = h->elements[i / 2];
    h->elements[i] = x;
    return 0;
}

void percolate_up(priority_query h, int index)
{
    int i = index;
    for (; h->elements[i / 2] > h->elements[i]; i /= 2)
    {
        ElementType temp = h->elements[i];
        h->elements[i] = h->elements[i / 2];
        h->elements[i / 2] = temp;
    }
}

void percolate_down(priority_query h, int index)
{
    int child, i;
    for (i = index; i * 2 <= h->size; i = child)
    {
        child = i * 2;
        if (child != h->size && h->elements[child + 1] < h->elements[child])
            child++;
        
        if (h->elements[i] > h->elements[child])
        {
            ElementType temp = h->elements[i];
            h->elements[i] = h->elements[child];
            h->elements[child] = temp;
        }
        else
            break;
    }
}

ElementType find_min(priority_query h)
{
    if (is_empty(h))
    {
        return h->elements[0];
    }
    else
        return h->elements[1];
}

int increase_key(int index, ElementType delta, priority_query h)
{
    if (delta < 0)
    {
        printf("delta is negative\n");
        return -1;
    }
    if (h == NULL)
    {
        printf("the priority query is NULL\n");
        return -1;
    }
    if (index > h->size || index < 1)
    {
        printf("out of range\n");
        return -1;
    }

    h->elements[index] += delta;
    percolate_down(h, index);

    return 0;
}

int print_priority_queue(priority_query h)
{
    if (h == NULL)
        return -1;
    
    for (int i = 1; i <= h->size; i++)
        printf("%d ", h->elements[i]);
    return 0; 
}

int create_array(int *array, int len, int max)
{
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    for (int i = 0; i < len; i++)
        array[i] = rand() % max + 1;
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

int max_box(ElementType *items, int item_size)
{
    priority_query h = initialize(MaxElementSize);
    if (h == NULL)
        return -1;
    
    int i = 0; 
    while (i < item_size)
    {
        printf("\n\nitem %d, the value: %d", i, items[i]);
        ElementType min_elem = find_min(h);
        if ((C - min_elem) >= items[i] && min_elem >= 0)
        {
            printf("\npush the item to minimal node with value: %d", h->elements[1]);
            if(increase_key(1, items[i], h) < 0)
                return -1;
            printf(", and the updated minimal node with value: %d", h->elements[1]);
            i++;
        }
        else
        {
            if (insert(0, h) < 0)
                return -1;
            printf("\ncreated a new node");
        }
    }
    printf("\nused %d boxes, the boxes heap is: \n", h->size);
    print_priority_queue(h);

}

int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    int items_size = 10;
    int items[items_size];
    create_array(items, items_size, 5);
    printf("array:\n");
    print_array(items, items_size);

    max_box(items, items_size);

    printf("\n");
    return 0;
}