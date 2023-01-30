#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define ElementType point
#define CutOff (3)
#define MAX 100000.0
#define Dist(a, b) sqrt(pow((a).x - (b).x, 2) + pow((a).y - (b).y, 2))

struct point_struct;
typedef struct point_struct point;
struct point_struct
{
    float x;
    float y;
};

int cmpfunc_x (const void *a, const void *b)
{
   return ((*(point*)a).x - (*(point*)b).x);
}

int cmpfunc_y (const void *a, const void *b)
{
   return ((*(point*)a).y - (*(point*)b).y);
}

void nearest_point(point pdata[], point qdata[], int data_size, float *min_dist, point *best_point1, point *best_point2)
{
    // float min_dist;

    if (data_size <= 1)
    {
        *min_dist = MAX;
        return;
    }
    float left = pdata[0].x;
    float right = pdata[data_size - 1].x;
    if (left == right)
    {
        *min_dist = MAX;
        for (int i = 0; i < data_size; i++)
            for (int j = i + 1; j < data_size; j++)
            {
                float temp = Dist(pdata[i], pdata[j]);
                if (temp < *min_dist)
                {
                    *min_dist = temp;
                    *best_point1 = pdata[i];
                    *best_point2 = pdata[j];
                }
            }
        return;
    }

    float center = (left + right) / 2;
    int left_size = 0;
    for (; left_size < data_size; left_size++)
        if (pdata[left_size].x >= center)
            break;
    
    int right_size = data_size - left_size;
    point pdata_left[left_size];
    point pdata_right[right_size];
    memcpy(pdata_left, pdata, sizeof(point) * left_size);
    memcpy(pdata_right, pdata + left_size, sizeof(point) * right_size);

    point qdata_left[left_size];
    point qdata_right[right_size];
    int l, r;
    for (int i = 0, l = 0, r = 0; i < data_size; i++)
        if (qdata[i].x < center)
            qdata_left[l++] = qdata[i];
        else
            qdata_right[r++] = qdata[i];
    

    float min_left;
    point left_bp1;
    point left_bp2;
    point right_bp1;
    point right_bp2;
    nearest_point(pdata_left, qdata_left, left_size, &min_left, &left_bp1, &left_bp2);
    float min_right;
    nearest_point(pdata_right, qdata_right, right_size, &min_right, &right_bp1, &right_bp2);
    if (min_left < min_right)
    {
        *min_dist = min_left;
        *best_point1 = left_bp1;
        *best_point2 = left_bp2;
    }
    else
    {
        *min_dist = min_right;
        *best_point1 = right_bp1;
        *best_point2 = right_bp2;
    }

    int data_strip_size = 0;
    point qdata_strip[data_size];
    for (int i = 0; i < data_size; i++)
        if (abs(qdata[i].x - center) < *min_dist)
            qdata_strip[data_strip_size++] = qdata[i];
    
    for (int i = 0; i < data_strip_size; i++)
        for (int j = i + 1; j < data_strip_size; j++)
            if (abs(qdata_strip[i].y - qdata_strip[j].y) >= *min_dist)
                break;
            else
            {
                float temp = Dist(qdata_strip[i], qdata_strip[j]);
                if (temp < *min_dist)
                {
                    *min_dist = temp;
                    *best_point1 = qdata_strip[i];
                    *best_point2 = qdata_strip[j];
                }
            }
    // return min_dist;
}

int print_point(point *array, int len)
{
    for (int i = 0; i < len; i++)
        printf("\n%f, %f", array[i].x, array[i].y);
    printf("\n");
    return 0;
}

void find_nearest_point(point *data, int data_size)
{
    point data_y[data_size];
    memcpy(data_y, data, sizeof(point) * data_size);
    qsort(data, data_size, sizeof(point), cmpfunc_x);
    qsort(data_y, data_size, sizeof(point), cmpfunc_y);
    print_point(data, data_size);
    float dist;
    point best_point1;
    point best_point2;
    nearest_point(data, data_y, data_size, &dist, &best_point1, &best_point2);
    printf("nearest dist: %f, point1: (%f, %f), point2: (%f, %f)", dist, best_point1.x, best_point1.y, best_point2.x, best_point2.y);
}

int create_point_array(point *array, int len, int max)
{
    for (int i = 0; i < len; i++)
    {
        int r1 = rand() % max;
        int r2 = rand() % max;
        array[i].x = (float)r1;
        array[i].y = (float)r2;
    }
    return 0;
}

int main()
{
    srand(time(NULL)); // Initialization, should only be called once.
    int data_size = 10;
    point data[data_size];
    create_point_array(data, data_size, 30);
    
    find_nearest_point(data, data_size);
    printf("\n");
    return 0;
}