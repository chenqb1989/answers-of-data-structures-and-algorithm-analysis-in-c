/*
note:
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int
#define CutOff (3)

void insertion_sort(ElementType array[], int size)
{
    ElementType tmp;
    for (int p = 1; p < size; p++)
    {
        tmp = array[p];
        int j;
        for (j = p; j > 0 && array[j - 1] > tmp; j--)
            array[j] = array[j - 1];
        array[j] = tmp;
    }
}

void swap(ElementType *v1, ElementType *v2)
{
    ElementType tmp = *v1;
    *v1 = *v2;
    *v2 = tmp;
}

ElementType median3(ElementType array[], int left, int right)
{
    int center = (left + right) / 2;

    if (array[left] > array[center])
        swap(&array[left], &array[center]);
    if (array[left] > array[right])
        swap(&array[left], &array[right]);
    if (array[center] > array[right])
        swap(&array[center], &array[right]);
    
    swap(&array[center], &array[right - 1]); // hide pivot;
    return array[right - 1];
}

void q_select(ElementType array[], int k, int left, int right)
{
    if (left + CutOff <= right)
    {
        ElementType pivot = median3(array, left, right);
        int i = left; 
        int j = right - 1;
        for (; ;)
        {
            while (array[++i] < pivot);
            while (array[--j] > pivot);
            if (i < j)
                swap(&array[i], &array[j]);
            else
                break;
        }
        swap(&array[i], &array[right - 1]);  // restore pivot
        if (i >= k)
            q_select(array, k, left, i - 1);
        else if (i + 1 < k)
            q_select(array, k, i + 1, right);
    }
    else
        insertion_sort(array + left, right - left + 1);
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
    // srand(time(NULL)); // Initialization, should only be called once.
    int number = 13;
    int array[13] = {81, 94, 11, 96, 12, 35, 17, 95, 28, 58, 41, 75, 15};
    // create_array(array, number);
    printf("array:\n");
    print_array(array, number);

    for (int k = 1; k < number + 1; k++)
    {
        q_select(array, k, 0, number - 1);
        printf("\n %dth smallest element: %d ", k, array[k - 1]);
    }
    
    printf("\n");
    return 0;
}