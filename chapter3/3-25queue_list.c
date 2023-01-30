/*
note:
1. note the stuct of queue and node
2. if use list, then need to malloc and free each node
3. if use array, then need to check full_queue, so need to add capacity in struct
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int

struct t_node;
typedef struct t_node *ptr_to_node;
typedef ptr_to_node t_list;

struct t_node
{
    ElementType element;
    ptr_to_node next;
};

struct queue_record;
typedef struct queue_record *t_queue;

struct queue_record
{
    ptr_to_node rear;
    t_list list;
};

int is_empty(t_queue que)
{
    return que->list->next == NULL;
}

t_queue create_queue()
{
    t_queue que = malloc(sizeof(struct queue_record));
    if (que == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    que->list = malloc(sizeof(struct t_node));
    if (que->list == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    que->rear = que->list;
    return que;
}

int enqueue(t_queue que, ElementType x)
{
    ptr_to_node tmp_cell = malloc(sizeof(struct t_node));
    if (tmp_cell == NULL)
    {
        printf("out of space!\n");
        return -1;
    }
    tmp_cell->element = x;
    tmp_cell->next = que->rear->next;
    que->rear->next = tmp_cell;
    que->rear = que->rear->next;
    return 0;
}

ElementType dequeue(t_queue que)
{
    if (is_empty(que))
    {
        printf("queue is empty!\n");
        return -1;
    }
    ptr_to_node front = que->list->next;
    ElementType result = front->element;
    que->list->next = front->next;
    free(front);
    return result;
}

int free_queue(t_queue que)
{
    if (que == NULL)
        return 0;
    ptr_to_node pos = que->list;
    while (pos != NULL)
    {
        ptr_to_node del_p = pos;
        pos = pos->next;
        free(del_p);
    }
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
            if (enqueue(que, tmp) == -1)
                break;
            printf("enque: %d\n", tmp);
        }
        else
        {
            int tmp;
            if ((tmp = dequeue(que)) == -1)
                break;
            printf("deque: %d\n", tmp);
        }
            
    }
    // printf("\nqueue: ");
    // while(!is_empty(que))
    // {
    //     int tmp;
    //     if ((tmp = dequeue(que)) == -1)
    //         break;
    //     printf("%d ", tmp);
    // }
    printf("\n");
    free_queue(que);
}
