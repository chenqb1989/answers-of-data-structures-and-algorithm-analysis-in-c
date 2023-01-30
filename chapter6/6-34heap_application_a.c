/*
note:
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define ElementType int
#define C 5

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

void first_box(ElementType *boxes, ElementType *items, int boxes_size, int items_size)
{
    for (int i = 0; i < items_size; i++)
    {
        int j = 0;
        for (; j < boxes_size; j++)
        {
            if (items[i] <= boxes[j])
            {
                boxes[j] -= items[i];
                printf("\nput item %d into box %d", i, j);
                break;
            }
        }
        if (j == boxes_size)
        {
            printf("\nitem %d is bigger than any box size", i);
        }
    }

    int i = 0;
    for (; i < boxes_size; i++)
        if (boxes[i] == 5)
        {
            printf("\nused %d boxes", i);
            break;
        }
    
    if (i == boxes_size)
        printf("\nused all boxes");
    
}

int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    int items_size = 10;
    int items[items_size];
    create_array(items, items_size, 5);
    printf("array:\n");
    print_array(items, items_size);

    int boxes_size = 10;
    ElementType boxes[boxes_size];
    for (int i = 0; i < boxes_size; i++)
        boxes[i] = C;
    
    first_box(boxes, items, boxes_size, items_size);

    printf("\n");
    return 0;
}