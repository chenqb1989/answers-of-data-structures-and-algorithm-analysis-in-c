/*
note:
1. the condition to determine full_stack or empty_stack
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ElementType int
#define EmptyToS (-1)
#define MinStackSize (5)

struct stack_record;
typedef struct stack_record *t_stack;
struct double_stack_record;
typedef struct double_stack_record *t_double_stack;


struct stack_record
{
    int top_of_stack;
};

struct double_stack_record
{
    int capacity;
    t_stack forward;
    t_stack backward;
    ElementType *data;
};

int is_empty_forward(t_double_stack double_stk)
{
    return (double_stk->forward->top_of_stack == EmptyToS);
}

int is_empty_backward(t_double_stack double_stk)
{
    return (double_stk->backward->top_of_stack == double_stk->capacity);
}

t_double_stack create_stack(int max_elements)
{
    if (max_elements < MinStackSize)
    {
        printf("stack size is too small!\n");
        return NULL;
    }

    t_double_stack double_stk = malloc(sizeof(struct double_stack_record));
    if (double_stk == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    double_stk->data = malloc(sizeof(ElementType) * max_elements);
    if (double_stk->data == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    double_stk->forward = malloc(sizeof(struct stack_record));
    if (double_stk->forward == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    double_stk->backward = malloc(sizeof(struct stack_record));
    if (double_stk->backward == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    double_stk->capacity = max_elements;
    double_stk->forward->top_of_stack = -1;
    double_stk->backward->top_of_stack = max_elements;
    return double_stk;
}

int push_forward(t_double_stack double_stk, ElementType x)
{
    if (double_stk->forward->top_of_stack >= (double_stk->backward->top_of_stack - 1))
    {
        printf("stack is full!\n");
        return -1;
    }
    double_stk->data[++(double_stk->forward->top_of_stack)] = x;
    return 0;
}

int push_backward(t_double_stack double_stk, ElementType x)
{
    if (double_stk->forward->top_of_stack >= (double_stk->backward->top_of_stack - 1))
    {
        printf("stack is full!\n");
        return -1;
    }
    double_stk->data[--(double_stk->backward->top_of_stack)] = x;
    return 0;
}

int pop_forward(t_double_stack double_stk)
{
    if (is_empty_forward(double_stk) == 1)
    {
        printf("empty stack!\n");
        return -1;
    }
    (double_stk->forward->top_of_stack)--;
    return 0;
}

int pop_backward(t_double_stack double_stk)
{
    if (is_empty_backward(double_stk) == 1)
    {
        printf("empty stack!\n");
        return -1;
    }
    (double_stk->backward->top_of_stack)++;
    return 0;
}

ElementType top_forward(t_double_stack double_stk)
{
    if (is_empty_forward(double_stk))
    {
        printf("empty stack!\n");
        return 0;
    }
    return double_stk->data[double_stk->forward->top_of_stack];
}

ElementType top_backward(t_double_stack double_stk)
{
    if (is_empty_backward(double_stk))
    {
        printf("empty stack!\n");
        return 0;
    }
    return double_stk->data[double_stk->backward->top_of_stack];
}

int free_stack(t_double_stack double_stk)
{
    if (double_stk == NULL)
        return 0;
    free(double_stk->forward);
    free(double_stk->backward);
    free(double_stk->data);
    free(double_stk);
}

int create_array(int *array, int len)
{
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    // for (int i = 0; i < len; i++)
    //     array[i] = i;
    for (int i = 0; i < len; i++)
    {
        int r = rand() % 10;
        // int tmp = array[i];
        // array[i] = array[r];
        array[i] = r;
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
    srand(time(NULL));   // Initialization, should only be called once.
    int number = 10;
    int array1[10]; 
    if (create_array(array1, number) == -1)
        return -1;
    printf("\narray1: ");
    print_array(array1, number);
    int array2[10]; 
    if (create_array(array2, number) == -1)
        return -1;
    printf("\narray2: ");
    print_array(array2, number);
    // t_double_stack db_stk = create_stack(10);
    t_double_stack db_stk = create_stack(100);
    for (int i = 0; i < number; i++)
    {
        if (push_forward(db_stk, array1[i]) == -1)
            break;
        if (push_backward(db_stk, array2[i]) == -1)
            break;
    }
    printf("\nforward stack: ");
    for (int i = 0; i < number; i++)
    {
        int tmp = top_forward(db_stk);
        printf("%d ", tmp);
        if (pop_forward(db_stk) == -1)
            break;
    }
    printf("\nbackward stack: ");
    for (int i = 0; i < number; i++)
    {
        int tmp = top_backward(db_stk);
        printf("%d ", tmp);
        if (pop_backward(db_stk) == -1)
            break;
    }
    printf("\n");
    free_stack(db_stk);
}