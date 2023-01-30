/*
note:
1. I don't understand how to do it, i have a problem, when deleting max, percolate up might terminate on a immidiate node, 
    and nodes between it and the root might break heap rules. So, the result of heap_sort2 is wrong!!!
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define ElementType int
#define LeftChild(i) (2 * (i) + 1)
#define Father(i) (((i) - 1) / 2)


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

void swap(ElementType *v1, ElementType *v2)
{
    ElementType tmp = *v1;
    *v1 = *v2;
    *v2 = tmp;
}

void percolate_down(ElementType array[], int i, int size)
{
    int child;
    ElementType tmp;
    for (tmp = array[i]; LeftChild(i) < size; i = child)
    {
        child = LeftChild(i);
        if (child != size - 1 && array[child + 1] > array[child])
            child++;
        
        if (tmp < array[child])
            array[i] = array[child];
        else
            break;
    }
    array[i] = tmp;
}

void percolate_down2(ElementType array[], int i, int size)
{
    int child, father;
    ElementType tmp;
    for (tmp = array[i]; LeftChild(i) < size; i = child)
    {
        child = LeftChild(i);
        array[i] = array[child];
    }
    array[i] = tmp;

    
    if (i != size - 1 && array[i + 1] > array[i])
        i++;
    
    // percolate_up
    for (tmp = array[i]; i > 0; i = father)
    {
        father = Father(i);
        if (array[father] < tmp)
            array[i] = array[father];
        else
            break;
    }
    array[i] = tmp;
}


void heap_sort(ElementType array[], int size)
{
    int i;
    for (i = size / 2; i >= 0; i--)
        percolate_down(array, i, size);  // build heap
    
    for (i = size - 1; i > 0; i--)
    {
        swap(&array[0], &array[i]);  // delete max
        percolate_down(array, 0, i);
    }
}

void heap_sort2(ElementType array[], int size)
{
    int i;
    for (i = size / 2 - 1; i >= 0; i--)
        percolate_down2(array, i, size);  // build heap
    
    printf("\nbuild heap: ");
    print_array(array, size);
    for (i = size - 1; i > 0; i--)
    {
        swap(&array[0], &array[i]);  // delete max
        percolate_down2(array, 0, i);
        printf("\ni %d: ", i);
        print_array(array, size);
    }
}

int main()
{
    srand(time(NULL)); // Initialization, should only be called once.
    int number = 13;
    int array[13] = {81, 94, 11, 96, 12, 35, 17, 95, 28, 58, 41, 75, 15};
    // int array[number];
    // create_array(array, number);
    printf("array:\n");
    print_array(array, number);

    heap_sort2(array, number);
    printf("\nsorted array:\n");
    print_array(array, number);

    printf("\n");
    return 0;
}