/*
note:
1. note that vertex has no name in this model, just for simplicity.
2. note that indegree is saved in the header of list(adjacent edges) of each vertex
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ElementType int
#define MaxVertexNum 100

struct t_node;
typedef struct t_node *ptr_to_edge;

struct t_node
{
    ElementType element;
    ptr_to_edge next;
};

/* ******edge func****** */
int insert_edge(int x, ptr_to_edge p)
{
    ptr_to_edge tmp_cell = (ptr_to_edge)malloc(sizeof(struct t_node));
    if (tmp_cell == NULL)
        return -1;
    tmp_cell->element = x;
    tmp_cell->next = p->next;
    p->next = tmp_cell;
    return 0;
}

void init_edge(ptr_to_edge lst)
{
    lst->element = 0;
    lst->next = NULL;
}

void print_edges(ptr_to_edge l)
{
    ptr_to_edge p = l->next;
    while (p != NULL)
    {
        printf(" %d", p->element);
        p = p->next;
    }
}

int free_edge(ptr_to_edge lst)
{
    ptr_to_edge del_p;
    while (lst != NULL)
    {
        del_p = lst;
        lst = lst->next;
        free(del_p);
    }
    free(lst);
    return 0;
}

/* queue func */
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

int enqueue(t_queue que, ElementType x)
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

ElementType dequeue(t_queue que)
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

int free_queue(t_queue que)
{
    if (que == NULL)
        return 0;
    free(que->data);
    free(que);
}

/* ****** graph func ******* */

struct t_graph;
typedef struct t_graph *ptr_to_graph;

struct t_graph
{
    int vertex_num;
    ptr_to_edge *edges;
};

ptr_to_graph create_graph(int vertex_num, ElementType edges[][2], int edge_num)
{
    if (vertex_num > MaxVertexNum)
    {
        printf("too many vertexes!\n");
        return NULL;
    }
    ptr_to_graph graph = malloc(sizeof(struct t_graph));
    if (graph == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
        
    graph->vertex_num = vertex_num;
    graph->edges = malloc(MaxVertexNum * sizeof(ptr_to_edge));
    if (graph->edges == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }

    for (int i = 0; i < vertex_num; i++)
    {
        graph->edges[i] = malloc(sizeof(struct t_node));
        if (graph->edges[i] == NULL)
        {
            printf("out of space!\n");
            return NULL;
        }
        init_edge(graph->edges[i]);
    }

    // read edges
    for (int i = 0; i < edge_num; i++)
    {
        graph->edges[edges[i][1] - 1]->element++;  // update ingree
        if (insert_edge(edges[i][1], graph->edges[edges[i][0] - 1]) < 0)
            return NULL;
    }

    return graph;
}

void top_sort(ptr_to_graph graph, int *top_num)
{
    t_queue queue = create_queue(graph->vertex_num);
    int counter = 0;

    for (int i = 0; i < graph->vertex_num; i++)
        if (graph->edges[i]->element == 0)
            enqueue(queue, i);
    
    while (!is_empty(queue))
    {
        int vertex = dequeue(queue);
        top_num[vertex] = ++counter;

        ptr_to_edge adjacent = graph->edges[vertex]->next;
        while (adjacent != NULL)
        {
            if (--graph->edges[adjacent->element - 1]->element == 0)
                enqueue(queue, adjacent->element - 1);
            adjacent = adjacent->next;
        }
    }
    if (counter != graph->vertex_num)
    {
        printf("graph has a cycle!\n");
    }
    free_queue(queue);
}

void print_graph(ptr_to_graph graph)
{
    if (graph != NULL)
        for (int i = 0; i < graph->vertex_num; i++)
        {
            printf("\nvertex: %d, indegree: %d, edges: ", i + 1, graph->edges[i]->element);
            print_edges(graph->edges[i]);
        }
}

void free_graph(ptr_to_graph graph)
{
    if (graph != NULL)
    {
        for (int i = 0; i < graph->vertex_num; i++)
            free_edge(graph->edges[i]);
        
        free(graph->edges);
        free(graph);
    }
}


int main()
{
    int vertex_num = 7;
    int edge_num = 12;
    ElementType edges[12][2] = {{1, 2}, {1, 3}, {1, 4}, {2, 4}, {2, 5}, {3, 6}, 
            {4, 3}, {4, 6}, {4, 7}, {5, 4}, {5, 7}, {7, 6}};
    ptr_to_graph graph = create_graph(vertex_num, edges, edge_num);
    print_graph(graph);

    int top_num[vertex_num];
    top_sort(graph, top_num);

    printf("\ntop num: ");
    for (int i = 0; i < vertex_num; i++)
        printf("%d ", top_num[i]);

    free_graph(graph);
    printf("\n");
}

