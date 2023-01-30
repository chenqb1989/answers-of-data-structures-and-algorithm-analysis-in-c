/*
note:
1. use array, we can get previous and next element; however, if using list, 
    it's harder to implement eject because of no way to get previous node, unless using bidirectional list
2. size is critical to avoide many possible errors, when push, pop, inject, eject; 
    we only need to be caruful of updating front and rear (using succ and minus)
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int
#define MinQueueSize (5)

struct queue_record;
typedef struct queue_record *t_queue;

struct queue_record
{
    int capacity;
    int front;
    int rear;
    int size;
    ElementType *data;
};

int is_empty(t_queue que)
{
    return que->size == 0;
}

t_queue create_queue(int max_elements)
{
    if (max_elements < MinQueueSize)
    {
        printf("queue size is too small!\n");
        return NULL;
    }
    t_queue que = malloc(sizeof(struct queue_record));
    if (que == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    que->data = malloc(sizeof(ElementType) * max_elements);
    if (que->data == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    que->front = 1;
    que->rear = 0;
    que->size = 0;
    que->capacity = max_elements;
    return que;
}

static int succ(int value, t_queue que)
{
    if (++value == que->capacity)
        value = 0;
    return value;
}

static int minus(int value, t_queue que)
{
    if (--value == -1)
        value = que->capacity - 1;
    return value;
}

int push(t_queue que, ElementType x)
{
    if (que->size == que->capacity)
    {
        printf("queue is full!\n");
        return -1;
    }
    que->front = minus(que->front, que);
    que->data[que->front] = x;
    que->size++;
    return 0;
}

ElementType pop(t_queue que)
{
    if (is_empty(que))
    {
        printf("queue is empty!\n");
        return -1;
    }
    ElementType result = que->data[que->front];
    que->front = succ(que->front, que);
    que->size--;
    return result;
}

int inject(t_queue que, ElementType x)
{
    if (que->size == que->capacity)
    {
        printf("queue is full!\n");
        return -1;
    }
    que->rear = succ(que->rear, que);
    que->data[que->rear] = x;
    que->size++;
    return 0;
}

ElementType eject(t_queue que)
{
    if (is_empty(que))
    {
        printf("queue is empty!\n");
        return -1;
    }
    ElementType result = que->data[que->rear];
    que->rear = minus(que->rear, que);
    que->size--;
    return result;
}

int free_queue(t_queue que)
{
    if (que == NULL)
        return 0;
    free(que->data);
    free(que);
}

int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    
    t_queue que = create_queue(10);
    for (int i = 0; i < 20; i++)
    {
        int is_enque = rand() % 2;
        if (i < 3)
            is_enque = 1;
        if (is_enque == 1)
        {
            int tmp = rand() % 100;
            int is_push = rand() % 2;
            if (is_push == 1)
            {
                if (push(que, tmp) == -1)
                    break;
                printf("enque_forward: %d\n", tmp);
            }
            else
            {
                if (inject(que, tmp) == -1)
                    break;
                printf("enque_backward: %d\n", tmp);
            }
        }
        else
        {
            int tmp;
            int is_pop = rand() % 2;
            if (is_pop == 1)
            {
                if ((tmp = pop(que)) == -1)
                    break;
                printf("deque_forward: %d\n", tmp);
            }
            else
            {
                if ((tmp = eject(que)) == -1)
                    break;
                printf("deque_backward: %d\n", tmp);
            }
            
        }
            
    }

    printf("\n");
    free_queue(que);
}
