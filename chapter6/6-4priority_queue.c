/*
note:
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define ElementType int
#define MaxValue __INT32_MAX__
#define MinPQSize 1
#define MinData -1

struct heap_struct;
typedef struct heap_struct *priority_queue;

struct heap_struct
{
    int capacity;
    int size;
    ElementType *elements;
};

priority_queue initialize(int max_elments)
{
    priority_queue h;

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

int is_full(priority_queue h)
{
    if (h->size == h->capacity)
        return 1;
    else
        return 0;
}

int is_empty(priority_queue h)
{
    if (h->size == 0)
        return 1;
    else
        return 0;
}

int insert(ElementType x, priority_queue h)
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

ElementType delete_min(priority_queue h)
{
    if (is_empty(h))
    {
        printf("the priority queue is empty!\n");
        return h->elements[0];
    }

    ElementType min_element = h->elements[1];
    ElementType last_element = h->elements[h->size--];
    int child, i;
    for (i = 1; i * 2 <= h->size; i = child) // note the termination condition
    {
        child = i * 2;
        if (child != h->size && h->elements[child + 1] < h->elements[child])
            child++;
        
        if (last_element > h->elements[child])
            h->elements[i] = h->elements[child];
        else
            break;
    }
    h->elements[i] = last_element;
    return min_element;
}

void percolate_up(priority_queue h, int index)
{
    int i = index;
    for (; h->elements[i / 2] > h->elements[i]; i /= 2)
    {
        ElementType temp = h->elements[i];
        h->elements[i] = h->elements[i / 2];
        h->elements[i / 2] = temp;
    }
}

void percolate_down(priority_queue h, int index)
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

priority_queue build_heap(ElementType *array, int array_size)
{
    priority_queue h = initialize(array_size);
    if (h == NULL)
        return NULL;
    
    memcpy(h->elements + 1, array, sizeof(ElementType) * array_size);
    h->size = array_size;
    for (int i = array_size / 2; i > 0; i--)
        percolate_down(h, i);
    
    return h;
}

ElementType find_min(priority_queue h)
{
    if (is_empty(h))
    {
        printf("the priority queue is empty!\n");
        return h->elements[0];
    }
    else
        return h->elements[1];
}

int decrease_key(int index, ElementType delta, priority_queue h)
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

    h->elements[index] -= delta;
    percolate_up(h, index);

    return 0;
}

int increase_key(int index, ElementType delta, priority_queue h)
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

int delete(int index, priority_queue h)
{
    if (decrease_key(index, MaxValue, h) < 0)
        return -1;
    
    ElementType buf = delete_min(h);
    return 0;
}

int print_priority_queue(priority_queue h)
{
    if (h == NULL)
        return -1;
    
    for (int i = 1; i <= h->size; i++)
        printf("%d ", h->elements[i]);
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
    int number = 10;
    int array[number];
    create_array(array, number);
    printf("array:\n");
    print_array(array, number);

    // test build_heap
    priority_queue h = build_heap(array, number);
    printf("\n initial priority heap: \n");
    print_priority_queue(h);

    // test delete_min
    delete_min(h);
    printf("\npriority heap after deleting min: \n");
    print_priority_queue(h);

    // test delete and decrease_key
    delete(5, h);
    printf("\npriority heap after deleting 5th node: \n");
    print_priority_queue(h);

    // test insert 0
    insert(0, h);
    printf("\npriority heap after inserting element with value 0: \n");
    print_priority_queue(h);
    
    // test increase_key
    increase_key(3, 10, h);
    printf("\npriority heap after increasing 10 to 3th node: \n");
    print_priority_queue(h);

    printf("\n");
    return 0;
}