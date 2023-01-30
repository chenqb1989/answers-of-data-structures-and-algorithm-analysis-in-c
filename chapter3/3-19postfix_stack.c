/*
note:
1. be careful of converting between string and float (line: 123-126)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ElementType char*
#define EmptyToS (-1)
#define MinStackSize (5)

struct stack_record;
typedef struct stack_record *t_stack;

struct stack_record
{
    int capacity;
    int top_of_stack;
    ElementType *data;
};

int is_empty(t_stack stk)
{
    return (stk->top_of_stack == EmptyToS);
}

t_stack create_stack(int max_elements)
{
    if (max_elements < MinStackSize)
    {
        printf("stack size is too small!\n");
        return NULL;
    }

    t_stack stk = malloc(sizeof(struct stack_record));
    if (stk == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    stk->data = malloc(sizeof(ElementType) * max_elements);
    if (stk->data == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    stk->capacity = max_elements;
    stk->top_of_stack = -1;
    return stk;
}

int push(t_stack stk, ElementType x)
{
    if (stk->top_of_stack >= (stk->capacity - 1))
    {
        printf("stack is full!\n");
        return -1;
    }
    stk->data[++(stk->top_of_stack)] = x;
    return 0;
}

int pop(t_stack stk)
{
    if (is_empty(stk) == 1)
    {
        printf("empty stack!\n");
        return -1;
    }
    (stk->top_of_stack)--;
    return 0;
}

ElementType top(t_stack stk)
{
    if (is_empty(stk))
    {
        printf("empty stack!\n");
        return NULL;
    }
    return stk->data[stk->top_of_stack];
}

ElementType top_pop(t_stack stk)
{
    if (is_empty(stk))
    {
        printf("empty stack!\n");
        return NULL;
    }
    return stk->data[(stk->top_of_stack)--];
}

int free_stack(t_stack stk)
{
    if (stk == NULL)
        return 0;
    free(stk->data);
    free(stk);
}

int postfix(ElementType *array, int len)
{
    if (array == NULL || len <= 0)
        return -1;
    t_stack stk = create_stack(100);
    for (int i = 0; i < len; i++)
    {
        ElementType new_cell = array[i];
        if (strcmp(new_cell, "+") == 0 || \
            strcmp(new_cell, "-") == 0 || \
            strcmp(new_cell, "*") == 0 || \
            strcmp(new_cell, "/") == 0)
        {
            ElementType operand1;
            ElementType operand2;
            if ((operand1 = top_pop(stk)) != NULL && (operand2 = top_pop(stk)) != NULL)
            {
                ElementType operand3;
                if (strcmp(new_cell, "+") == 0)
                {
                    float temp = atof(operand2) + atof(operand1);
                    int len = snprintf(NULL, 0, "%f", temp);
                    operand3 = malloc(len + 1);
                    snprintf(operand3, len + 1, "%f", temp);
                    if (push(stk, operand3) == -1)
                        return -1;
                }
                else if (strcmp(new_cell, "-") == 0)
                {
                    float temp = atof(operand2) - atof(operand1);
                    int len = snprintf(NULL, 0, "%f", temp);
                    operand3 = malloc(len + 1);
                    snprintf(operand3, len + 1, "%f", temp);
                    if (push(stk, operand3) == -1)
                        return -1;
                }
                else if (strcmp(new_cell, "*") == 0)
                {
                    float temp = atof(operand2) * atof(operand1);
                    int len = snprintf(NULL, 0, "%f", temp);
                    operand3 = malloc(len + 1);
                    snprintf(operand3, len + 1, "%f", temp);
                    if (push(stk, operand3) == -1)
                        return -1;
                }
                else if (strcmp(new_cell, "/") == 0)
                {
                    if (atof(operand1) == 0.f)
                    {
                        printf("zero dividor!\n");
                        return -1;
                    }
                    float temp = atof(operand2) / atof(operand1);
                    int len = snprintf(NULL, 0, "%f", temp);
                    operand3 = malloc(len + 1);
                    snprintf(operand3, len + 1, "%f", temp);
                    if (push(stk, operand3) == -1)
                        return -1;
                }
                    
            }
            else
            {
                printf("too few operands!\n");
                return -1;
            }
        }
        else
        {
            if (push(stk, new_cell) == -1)
                return -1;
        }
    }
    printf("result: %f\n", atof(top(stk)));
    free_stack(stk);
}

int main()
{
    int array_len = 11;
    // char *array[9] = {"4.99", "1.06", "*", "5.99", "+", "6.99", "1.06", "*", "+"};
    char *array[11] = {"6", "5", "2", "3", "+", "8", "*", "+", "3", "+", "*"};
    postfix(array, array_len);
}