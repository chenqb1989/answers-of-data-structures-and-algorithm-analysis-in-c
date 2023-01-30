/*
note:
1. be careful of right bracket and left bracket
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

int get_priority(const char *s)
{
    if (strcmp(s, "+") == 0 || strcmp(s, "-") == 0)
        return 3;
    else if (strcmp(s, "*") == 0 || strcmp(s, "/") == 0)
        return 4;
    else if (strcmp(s, "(") == 0 || strcmp(s, ")") == 0)
        return 10;
    else if (strcmp(s, "exp") == 0)
        return 6;
    else
    {
        printf("unsupported operator: %s", s);
        return -1;
    }
}

int is_rightfirst(char *s)
{
    if (strcmp(s, "exp") == 0)
        return 1;
    else
        return 0;
}

int infix_to_postfix(ElementType *array, int len, ElementType *out)
{
    t_stack stk = create_stack(100);
    int out_id = 0;
    ElementType operator;
    for (int i = 0; i < len; i++)
    {
        ElementType new_cell = array[i];
        if (strcmp(new_cell, "+") == 0 || \
            strcmp(new_cell, "-") == 0 || \
            strcmp(new_cell, "*") == 0 || \
            strcmp(new_cell, "/") == 0 || \
            strcmp(new_cell, "(") == 0 || \
            strcmp(new_cell, "exp") == 0)
        {
            if (is_empty(stk))
            {
                push(stk, new_cell);
            }
            else
            {
                operator = top(stk);
                int p1 = get_priority(operator);
                int p2 = get_priority(new_cell);
                int r2 = is_rightfirst(new_cell);
                while ((p1 > p2 || (p1 == p2 && r2 == 0)) && \
                        strcmp(operator, "(") != 0)
                {
                    out[out_id++] = operator;
                    pop(stk);
                    if (is_empty(stk))
                        break;
                    operator = top(stk);
                }
                push(stk, new_cell);
            }
        }
        else if (strcmp(new_cell, ")") == 0)
        {
            if (is_empty(stk))
            {
                printf("there is no corresponding left bracket for right bracket\n");
                return -1;
            }
            operator = top(stk);
            while(strcmp(operator, "(") != 0)
            {
                out[out_id++] = operator;
                pop(stk);
                if (is_empty(stk))
                {
                    printf("there is no corresponding left bracket for right bracket\n");
                    return -1;
                }
                operator = top(stk);
            }
            pop(stk); // pop the "("
        }
        else
        {
            // operand
            out[out_id++] = new_cell;
        }
    }
    while(!is_empty(stk))
    {
        operator = top_pop(stk);
        out[out_id++] = operator;
    }
    free_stack(stk);
    return out_id;
}

int main()
{
    int array_len = 21;
    char *array[21] = {"6", "+", "5", "*", "3", "+", "(", "8", "*", "3", "+", "2", ")", "*", "10", "+", "2", "exp", "3", "exp", "4"};
    char *postfix[21];
    int postfix_len = infix_to_postfix(array, array_len, postfix);
    if (postfix_len < 0)
        return -1;
    for (int i = 0; i < postfix_len; i++)
        printf("%s ", postfix[i]);
    printf("\n");
}