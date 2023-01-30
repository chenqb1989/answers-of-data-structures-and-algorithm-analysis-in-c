/*
note:
1. every qsort (except qsort_first, because pivot is hided naturally) need to hide and restore pivot, but the ways are different
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

// qsort, use median3 as pivot
void qsort_median3(ElementType array[], int left, int right, int cutoff)
{
    if (left + cutoff <= right)
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
        qsort_median3(array, left, i - 1, cutoff);
        qsort_median3(array, i + 1, right, cutoff);
    }
    else
        insertion_sort(array + left, right - left + 1);
}

// qsort, use first element as pivot
void qsort_first(ElementType array[], int left, int right, int cutoff)
{
    if (left + cutoff <= right)
    {
        ElementType pivot = array[left];
        int i = left; 
        int j = right + 1;
        for (; ;)
        {
            while (++i <= right && array[i] < pivot);
            while (--j > left && array[j] > pivot);
            if (i < j)
                swap(&array[i], &array[j]);
            else
                break;
        }
        swap(&array[j], &array[left]);  // j is the rightfirst element smaller than pivot
        qsort_first(array, left, j - 1, cutoff);
        qsort_first(array, j + 1, right, cutoff);
    }
    else
        insertion_sort(array + left, right - left + 1);
}

// qsort, use median element as pivot
void qsort_median(ElementType array[], int left, int right, int cutoff)
{
    if (left + cutoff <= right)
    {
        ElementType pivot = array[(left + right) / 2];
        swap(&array[(left + right) / 2], &array[right]); // hide pivot
        int i = left - 1; 
        int j = right;
        for (; ;)
        {
            while (++i < right && array[i] < pivot);
            while (--j >= left && array[j] > pivot);
            if (i < j)
                swap(&array[i], &array[j]);
            else
                break;
        }
        swap(&array[i], &array[right]);  // restore pivot
        qsort_median(array, left, i - 1, cutoff);
        qsort_median(array, i + 1, right, cutoff);
    }
    else
        insertion_sort(array + left, right - left + 1);
}

// qsort, use random element as pivot
void qsort_random(ElementType array[], int left, int right, int cutoff)
{
    if (left + cutoff <= right)
    {
        int r = rand() % (right - left + 1) + left;
        ElementType pivot = array[r];
        swap(&array[r], &array[right]); // hide pivot
        int i = left - 1; 
        int j = right;
        for (; ;)
        {
            while (++i < right && array[i] < pivot);
            while (--j >= left && array[j] > pivot);
            if (i < j)
                swap(&array[i], &array[j]);
            else
                break;
        }
        swap(&array[i], &array[right]);  // restore pivot
        qsort_random(array, left, i - 1, cutoff);
        qsort_random(array, i + 1, right, cutoff);
    }
    else
        insertion_sort(array + left, right - left + 1);
}

ElementType median5(ElementType array[], int left, int right)
{
    int center = (left + right) / 2;
    int upper_quartile = left + (right - left + 1) * 3 / 4;
    int lower_quartile = left + (right - left + 1) / 4;
    ElementType tmp[5] = {array[left], array[lower_quartile], array[center], array[upper_quartile], array[right]};
    insertion_sort(tmp, 5);
    array[left] = tmp[0];
    array[lower_quartile] = tmp[1];
    array[center] = tmp[2];
    array[upper_quartile] = tmp[3];
    array[right] = tmp[4];
    
    swap(&array[center], &array[right - 1]); // hide pivot;
    return array[right - 1];
}

// qsort, use median5 element as pivot
void qsort_median5(ElementType array[], int left, int right, int cutoff)
{
    if (left + cutoff <= right)
    {
        ElementType pivot = median5(array, left, right);
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
        qsort_median5(array, left, i - 1, cutoff);
        qsort_median5(array, i + 1, right, cutoff);
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
    srand(time(NULL)); // Initialization, should only be called once.
    int number = 30;
    // int array[13] = {81, 94, 11, 96, 12, 35, 17, 95, 28, 58, 41, 75, 15};
    int array[number];
    create_array(array, number);
    printf("array:\n");
    print_array(array, number);

    qsort_median3(array, 0, number - 1, 2);
    printf("\nsorted array:\n");
    print_array(array, number);

    printf("\n");
    return 0;
}