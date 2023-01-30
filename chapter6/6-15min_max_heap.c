/*
note:
1. insert, deal with based on heap depth, after inserting, check if need to percolate up even or odd
2. delete_min, 
    (1) the empty hole need to compare with its grandson
    (2) note to deal with last layer independently (line: 280-296)
3. build_heap, 
    (1) each node (id from N/2 to 1) need to compare with its son and grandson
    (2) note to compare with its son firstly, then compare to its grandson (line 308, 339).
    (3) percolate down recursively
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

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

double Log2( double n )  
{  
    // log(n)/log(2) is log2.  
    return log( n ) / log( 2 );  
}


void percolate_up_even(priority_queue h, int index)
{
    int i = index;
    for (; h->elements[i / 4] > h->elements[i]; i /= 4)
    {
        ElementType temp = h->elements[i];
        h->elements[i] = h->elements[i / 4];
        h->elements[i / 4] = temp;
    }
}

void percolate_up_odd(priority_queue h, int index)
{
    int i = index;
    for (; i > 4 && h->elements[i / 4] < h->elements[i]; i /= 4)
    {
        ElementType temp = h->elements[i];
        h->elements[i] = h->elements[i / 4];
        h->elements[i / 4] = temp;
    }

}

int insert(ElementType x, priority_queue h)
{
    if (is_full(h))
    {
        printf("the priority queue is full\n");
        return -1;
    }

    int i = ++h->size;
    int depth = (int)log2(i);
    if (depth == 0)
    {
        h->elements[i] = x;
    }
    else if (depth == 1)
    {
        if (h->elements[1] > x)
        {
            h->elements[i] = h->elements[1];
            h->elements[1] = x;
        }
        else
            h->elements[i] = x;
    }
    else
    {
        ElementType father = h->elements[i / 2];
        ElementType grandfather = h->elements[i / 4];
        if (depth % 2 == 0)
        {
            // even
            if (x > father)
            {
                h->elements[i] = father;
                h->elements[i / 2] = x;
                if (depth >= 4)
                    percolate_up_odd(h, i / 2);
            }
            else if (x < grandfather)
            {
                h->elements[i] = grandfather;
                h->elements[i / 4] = x;
                if (depth >= 4)
                    percolate_up_even(h, i / 4);
            }
            else
            {
                h->elements[i] = x;
            }

        }
        else
        {
            // odd
            if (x < father)
            {
                h->elements[i] = father;
                h->elements[i / 2] = x;
                if (depth >= 3)
                    percolate_up_even(h, i / 2);
            }
            else if (x > grandfather)
            {
                h->elements[i] = grandfather;
                h->elements[i / 4] = x;
                if (depth >= 5)
                    percolate_up_odd(h, i / 4);
            }
            else
            {
                h->elements[i] = x;
            }
        }
    }
    return 0;
}

ElementType delete_max(priority_queue h)
{
    if (is_empty(h))
    {
        printf("the priority queue is empty!\n");
        return h->elements[0];
    }
    if (h->size <= 2)  // only one or two element
        return h->elements[h->size--];
    
    int child, i, max_child;
    ElementType max_element;
    if (h->elements[2] > h->elements[3])
    {
        max_element = h->elements[2];
        max_child = 2;
    }
    else
    {
        max_element = h->elements[3];
        max_child = 3;
    }
    if (h->size == 3 && max_child == 3)  // tree nodes and the last is largest
        return h->elements[h->size--];

    ElementType last_element = h->elements[h->size--];
    for (i = max_child; i * 2 <= h->size; i = max_child) // note the termination condition
    {
        if (i * 4 <= h->size)  // compare with largest grandson
        {
            child = i * 4;
            ElementType max_child_elem = h->elements[child];
            max_child = child;
            for (int j = 0; j < 3 && child != h->size; child++, j++)
            {
                if (h->elements[child + 1] > max_child_elem)
                {
                    max_child_elem = h->elements[child + 1];
                    max_child = child + 1;
                }
            }
            if (last_element < max_child_elem)
                h->elements[i] = max_child_elem;
            else
                break;
        }
        else
        {
            // the last layer is with depth even, we should compare with largest son, 
            // because nodes with odd depth, should be large than its son.
            child = i * 2;            
            if (child != h->size && h->elements[child + 1] > h->elements[child])
                child++;
            
            if (last_element < h->elements[child])
            {
                h->elements[i] = h->elements[child];
                i = child;
                break;
            }
            else
                break;
        }
        
    }
    h->elements[i] = last_element;
    return max_element;
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
    int child, i, min_child;
    for (i = 1; i * 2 <= h->size; i = min_child) // note the termination condition
    {
        if (i * 4 <= h->size)
        {
            child = i * 4;
            ElementType min_child_elem = h->elements[child];
            min_child = child;
            for (int j = 0; j < 3 && child != h->size; child++, j++)
            {
                if (h->elements[child + 1] < min_child_elem)
                {
                    min_child_elem = h->elements[child + 1];
                    min_child = child + 1;
                }
            }
            if (last_element > min_child_elem)
                h->elements[i] = min_child_elem;
            else
                break;
        }
        else
        {
            // the last layer is with depth odd, we should compare with smallest son, 
            // because nodes with even depth, should be smaller than its son.
            child = i * 2;            
            if (child != h->size && h->elements[child + 1] < h->elements[child])
                child++;
            
            if (last_element > h->elements[child])
            {
                h->elements[i] = h->elements[child];
                i = child;
                break;
            }
            else
                break;
        }
    }
    h->elements[i] = last_element;
    return min_element;
}

void percolate_down(priority_queue h, int index, int depth)
{
    int child, i;
    i = index;

    if (i * 2 <= h->size)  // compare with its son
    {
        child = i * 2;
        if (depth % 2 == 0)
        {
            if (child != h->size && h->elements[child + 1] < h->elements[child])
                child++;
            
            if (h->elements[i] > h->elements[child])
            {
                ElementType temp = h->elements[i];
                h->elements[i] = h->elements[child];
                h->elements[child] = temp;
                percolate_down(h, child, depth - 1);
            }
        }
        else
        {
            if (child != h->size && h->elements[child + 1] > h->elements[child])
                child++;
            
            if (h->elements[i] < h->elements[child])
            {
                ElementType temp = h->elements[i];
                h->elements[i] = h->elements[child];
                h->elements[child] = temp;
                percolate_down(h, child, depth - 1);
            }
        }
    }

    if (i * 4 <= h->size) // compare with its grandson
    {
        child = i * 4;
        ElementType min_child_elem = h->elements[child];
        int min_child = child;

        if (depth % 2 == 0)
        {
            for (int j = 0; j < 3 && child != h->size; child++, j++)
            {
                if (h->elements[child + 1] < min_child_elem)
                {
                    min_child_elem = h->elements[child + 1];
                    min_child = child + 1;
                }
            }
            if (h->elements[i] > min_child_elem)
            {
                ElementType temp = h->elements[i];
                h->elements[i] = min_child_elem;
                h->elements[min_child] = temp;
                percolate_down(h, min_child, depth - 2);
            }
        }
        else
        {
            for (int j = 0; j < 3 && child != h->size; child++, j++)
            {
                if (h->elements[child + 1] > min_child_elem)
                {
                    min_child_elem = h->elements[child + 1];
                    min_child = child + 1;
                }
            }
            if (h->elements[i] < min_child_elem)
            {
                ElementType temp = h->elements[i];
                h->elements[i] = min_child_elem;
                h->elements[min_child] = temp;
                percolate_down(h, min_child, depth - 2);
            }
        }
        
    }
}

priority_queue build_heap(ElementType *array, int array_size)
{
    priority_queue h = initialize(array_size);
    if (h == NULL)
        return NULL;
    
    memcpy(h->elements + 1, array, sizeof(ElementType) * array_size);
    h->size = array_size;

    int depth = (int)log2(h->size);
    if (depth == 0)
    {
        return h;
    }
    
    for (int i = h->size / 2; i >= 0; i--)
    {
        depth = (int)log2(i);
        percolate_down(h, i, depth);
    }
    
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
    // srand(time(NULL));   // Initialization, should only be called once.
    int number = 25;
    int array[number];
    create_array(array, number);
    printf("array:\n");
    print_array(array, number);

    priority_queue h = initialize(100);

    for (int i = 0; i < number; i++)
        insert(array[i], h);
    printf("\ninitial priority heap: \n");
    print_priority_queue(h);

    for (int i = 0; i < number; i++)
    {
        int r = rand() % 2;
        if (r == 0)
        {
            delete_min(h);
            printf("\npriority heap after deleting_min: \n");
            print_priority_queue(h);
        }
        else
        {
            delete_max(h);
            printf("\npriority heap after deleting_max: \n");
            print_priority_queue(h);
        }
    }

    h = build_heap(array, number);
    printf("\nbuild_heap priority heap: \n");
    print_priority_queue(h);

    printf("\n");
    return 0;
}