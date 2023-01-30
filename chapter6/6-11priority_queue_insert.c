/*
note:
note the cosuming time are similar, when order are 'descend' and 'random' using 'build_heap'

the result:
mode: insert, order: ascend, time: 9.796875 
 
mode: build_heap, order: ascend, time: 9.906250 
 
mode: insert, order: descend, time: 45.046875 
 
mode: build_heap, order: descend, time: 17.390625 
 
mode: insert, order: random, time: 23.781250 
 
mode: build_heap, order: random, time: 16.468750 
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

int print_priority_queue(priority_queue h)
{
    if (h == NULL)
        return -1;
    
    for (int i = 1; i <= h->size; i++)
        printf("%d ", h->elements[i]);
    return 0; 
}

int create_array(int *array, int len, char *order)
{
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    if (strcmp(order, "descend") == 0)
        for (int i = 0; i < len; i++)
            array[i] = len - i - 1;
    else  // ascend and random
        for (int i = 0; i < len; i++)
            array[i] = i;
    
    if (strcmp(order, "random") == 0)
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


void test_insert(int size, int repeate_num, char *order, char *mode)
{
    ElementType array[size];
    create_array(array, size, order);

    priority_queue h = initialize(size + 1);

    clock_t start_t, finish_t;
    double total_t = 0;
    start_t = clock();

    if (strcmp(mode, "insert") == 0)
    {
        for (int i = 0; i < repeate_num; i++)
        {
            for (int j = 0; j < size; j++)
                insert(array[j], h);
            h->size = 0;
        }
        
        finish_t = clock();
        total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;
        printf(" \nmode: insert, order: %s, time: %f \n", order, total_t);
    }
    else
    {
        for (int i = 0; i < repeate_num; i++)
        {
            h = build_heap(array, size);
            h->size = 0;
        }
            
        
        finish_t = clock();
        total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;
        printf(" \nmode: build_heap, order: %s, time: %f \n", order, total_t);
    }
    
}

int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    int size = 1000;
    int repeat_time = 1000000;

    char *orders[3] = {"ascend", "descend", "random"};
    char *modes[2] = {"insert", "build_heap"};

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 2; j++)
            test_insert(size, repeat_time, orders[i], modes[j]);

    printf("\n");
    return 0;
}