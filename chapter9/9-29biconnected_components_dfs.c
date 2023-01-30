/*
note:
1. when deepfirst search in undirected graph, we should store (v, w) and (w, v) for each edge
2. use stack (important)
3. note when to push (line: 254, 278)
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MaxVertexNum (100)
#define EmptyToS (-1)
#define MaxStackSize (100)

/* struct t_edge */
struct t_edge;
typedef struct t_edge *ptr_to_edge;

struct t_edge
{
    int to_vertex;
    ptr_to_edge next;
};

/* struct single edge, just for saving and printing */
struct single_edge;
typedef struct single_edge *ptr_to_single_edge;
struct single_edge
{
    int to_vertex;
    int from_vertex;
};

/* struct stack */
struct stack_record;
typedef struct stack_record *t_stack;

struct stack_record
{
    int capacity;
    int top_of_stack;
    struct single_edge data[MaxStackSize];;
};

/* struct t_vertex */
struct t_vertex;
typedef struct t_vertex *ptr_to_vertex;
struct t_vertex
{
    int num;
    int low;
    int parent;
    char visited;
    char component;
};

/* struct t_graph */
struct t_graph;
typedef struct t_graph *ptr_to_graph;

struct t_graph
{
    int vertex_num;
    struct t_vertex vertexs[MaxVertexNum];
    ptr_to_edge *edges;
    int counter;
    t_stack single_edges;
};

/* ******edge func****** */
int insert_edge(int x, ptr_to_edge p)
{
    ptr_to_edge tmp_cell = (ptr_to_edge)malloc(sizeof(struct t_edge));
    if (tmp_cell == NULL)
        return -1;
    tmp_cell->to_vertex = x;
    tmp_cell->next = p->next;
    p->next = tmp_cell;
    return 0;
}

void init_edge(ptr_to_edge lst)
{
    lst->to_vertex = 0;
    lst->next = NULL;
}

void print_edges(ptr_to_edge l)
{
    ptr_to_edge p = l->next;
    while (p != NULL)
    {
        printf(" %d", p->to_vertex);
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

/* stack func */
int is_empty(t_stack stk)
{
    return (stk->top_of_stack == EmptyToS);
}

t_stack create_stack()
{
    t_stack stk = malloc(sizeof(struct stack_record));
    if (stk == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    stk->capacity = MaxStackSize;
    stk->top_of_stack = -1;
    return stk;
}

int push(t_stack stk, int from_vertex, int to_vertex)
{
    if (stk->top_of_stack >= (stk->capacity - 1))
    {
        printf("stack is full!\n");
        return -1;
    }
    ++(stk->top_of_stack);
    stk->data[stk->top_of_stack].from_vertex = from_vertex;
    stk->data[stk->top_of_stack].to_vertex = to_vertex;
    return 0;
}

ptr_to_single_edge toppop(t_stack stk)
{
    if (is_empty(stk) == 1)
    {
        printf("empty stack!\n");
        return NULL;
    }
    return &stk->data[(stk->top_of_stack)--];
}

int free_stack(t_stack stk)
{
    if (stk == NULL)
        return 0;
    free(stk);
}

/* ****** graph func ******* */
ptr_to_graph create_graph(int vertex_num, int edges[][2], int edge_num)
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

    // edge header of each vertex
    for (int i = 0; i < vertex_num; i++)
    {
        graph->edges[i] = malloc(sizeof(struct t_edge));
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
        if (insert_edge(edges[i][1], graph->edges[edges[i][0]]) < 0)
            return NULL;
    }
    // reverse, when deepfirst search in undirected graph, we should store (v, w) and (w, v) for each edge
    for (int i = 0; i < edge_num; i++)
    {
        if (insert_edge(edges[i][0], graph->edges[edges[i][1]]) < 0)
            return NULL;
    }

    for (int i = 0; i < graph->vertex_num; i++)
    {
        graph->vertexs[i].num = -1;
        graph->vertexs[i].visited = 0;
        graph->vertexs[i].component = -1;
        graph->vertexs[i].parent = -1;
    }
    graph->counter = 0;
    graph->single_edges = create_stack();
    return graph;
}

void print_graph(ptr_to_graph graph)
{
    if (graph != NULL)
        for (int i = 0; i < graph->vertex_num; i++)
        {
            printf("\nvertex: %d, visited: %d, num: %d, edges: ", i, graph->vertexs[i].visited, graph->vertexs[i].num);
            print_edges(graph->edges[i]);
        }
}

void free_graph(ptr_to_graph graph)
{
    if (graph != NULL)
    {
        for (int i = 0; i < graph->vertex_num; i++)
        {
            free_edge(graph->edges[i]);
        }
        free(graph->edges);
        free(graph);
    }
}

void find_bc(ptr_to_graph graph, int vertex)
{
    graph->vertexs[vertex].visited = 1;
    graph->vertexs[vertex].num = ++graph->counter;
    graph->vertexs[vertex].low = graph->vertexs[vertex].num;  // rule1
    ptr_to_edge w = graph->edges[vertex]->next;
    while (w != NULL)
    {
        if (graph->vertexs[w->to_vertex].visited == 0)
        {
            push(graph->single_edges, vertex, w->to_vertex);
            graph->vertexs[w->to_vertex].parent = vertex;
            find_bc(graph, w->to_vertex);
            if (graph->vertexs[w->to_vertex].low >= graph->vertexs[vertex].num)
            {
                // vertex is articulation point, stack pop until meet edge(v, w)
                printf("\n articulation point: %d, biconnected_commponents: ", vertex);
                while (1)
                {
                    ptr_to_single_edge se = toppop(graph->single_edges);
                    if (se == NULL)
                        break;
                    printf("%d to %d, ", se->from_vertex, se->to_vertex);
                    if (se->from_vertex == vertex && se->to_vertex == w->to_vertex)
                        break;
                }
            }
                // printf("\nvertex %d is an articulation point", vertex);
            if (graph->vertexs[vertex].low > graph->vertexs[w->to_vertex].low)
                graph->vertexs[vertex].low = graph->vertexs[w->to_vertex].low;  // rule 3
        }
        else if (graph->vertexs[vertex].parent != w->to_vertex)
            if (graph->vertexs[vertex].low > graph->vertexs[w->to_vertex].num)
            {
                push(graph->single_edges, vertex, w->to_vertex);
                graph->vertexs[vertex].low = graph->vertexs[w->to_vertex].num;  // rule 2
            }
                
        w = w->next;
    }
}

int main()
{
    int vertex_num = 7;
    int edge_num = 8;
    int edges[8][2] = {{0, 3}, {0, 1}, {1, 2}, {2, 6}, {2, 3}, {3, 5}, {3, 4}, {4, 5}};
    ptr_to_graph graph = create_graph(vertex_num, edges, edge_num);
    print_graph(graph);
    find_bc(graph, 0);
    free_graph(graph);
    printf("\n");
}

