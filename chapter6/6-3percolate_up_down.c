/*
note:
1. note the difference between temination conditions of 'percolate_up' and 'percolate_down'(line 56, 67)
    percolate_up don't need to check if i > 0, because the value at position 0 is MinData
2. in 'percolate_down', there is twice comparition, however, in 'percolate_up' there is only once, in each loop

*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int
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

int print_priority_queue(priority_queue h)
{
    if (h == NULL)
        return -1;
    
    for (int i = 1; i <= h->size; i++)
        printf("%d ", h->elements[i]);
    return 0; 
}



int main()
{
    int number = 5;
    priority_queue h = initialize(number);
    ElementType array[5] = {3, 54, 5, 2, 9};
    for (int i = 0; i < number; i++)
        h->elements[i + 1] = array[i];
    h->size = number;
    
    printf("\ninitial priority queue: ");
    print_priority_queue(h);

    // percolate_up(h, 4);
    // printf("\npriority queue after percolate up at element 4: ");
    // print_priority_queue(h);

    percolate_down(h, 2);
    printf("\npriority queue after percolate down: ");
    print_priority_queue(h);

    return 0;
}
