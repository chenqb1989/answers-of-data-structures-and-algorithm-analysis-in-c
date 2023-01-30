/*
note:
1. note that we can define array in struct (define struct t_vertex vertexs[MaxVertexNum] in struct t_graph)
2. we define edges and reverse_edges in struct t_graph, which is memory-consuming, but efficient
3. we define counter in struct t_graph, avoiding define it as global variable
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MaxVertexNum 100

/* struct t_edge */
struct t_edge;
typedef struct t_edge *ptr_to_edge;

struct t_edge
{
    int to_vertex;
    ptr_to_edge next;
};

/* struct t_vertex */
struct t_vertex;
typedef struct t_vertex *ptr_to_vertex;
struct t_vertex
{
    int num;
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
    ptr_to_edge *reverse_edges;
    int counter;
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

    graph->reverse_edges = malloc(MaxVertexNum * sizeof(ptr_to_edge));
    if (graph->reverse_edges == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    // reverse edge header of each vertex
    for (int i = 0; i < vertex_num; i++)
    {
        graph->reverse_edges[i] = malloc(sizeof(struct t_edge));
        if (graph->reverse_edges[i] == NULL)
        {
            printf("out of space!\n");
            return NULL;
        }
        init_edge(graph->reverse_edges[i]);
    }

    // read reverse edges
    for (int i = 0; i < edge_num; i++)
    {
        if (insert_edge(edges[i][0], graph->reverse_edges[edges[i][1]]) < 0)
            return NULL;
    }

    for (int i = 0; i < graph->vertex_num; i++)
    {
        graph->vertexs[i].num = -1;
        graph->vertexs[i].visited = 0;
        graph->vertexs[i].component = -1;
    }
    graph->counter = 0;
    return graph;
}

void print_graph(ptr_to_graph graph)
{
    if (graph != NULL)
        for (int i = 0; i < graph->vertex_num; i++)
        {
            printf("\nvertex: %d, visited: %d, num: %d, edges: ", i, graph->vertexs[i].visited, graph->vertexs[i].num);
            print_edges(graph->edges[i]);
            printf(", reverse edges: ");
            print_edges(graph->reverse_edges[i]);
        }
}

void free_graph(ptr_to_graph graph)
{
    if (graph != NULL)
    {
        for (int i = 0; i < graph->vertex_num; i++)
        {
            free_edge(graph->edges[i]);
            free_edge(graph->reverse_edges[i]);
        }
        free(graph->edges);
        free(graph->reverse_edges);
        free(graph);
    }
}

void assign_num_in(ptr_to_graph graph, int vertex)
{
    graph->vertexs[vertex].visited = 1;
    ptr_to_edge w = graph->edges[vertex]->next;
    while (w != NULL)
    {
        if (graph->vertexs[w->to_vertex].visited == 0)
            assign_num_in(graph, w->to_vertex);
        w = w->next;
    }
    graph->vertexs[vertex].num = ++graph->counter;
}

void assign_num(ptr_to_graph graph)
{
    while (graph->counter < graph->vertex_num)
    {
        int start = -1;
        for (int i = 0; i < graph->vertex_num; i++)
            if (graph->vertexs[i].visited == 0)
            {
                start = i;
                break;
            }
        
        assign_num_in(graph, start);
    }
}

void find_strong_component_in(ptr_to_graph graph, int vertex, int component)
{
    graph->vertexs[vertex].visited = 1;
    graph->vertexs[vertex].component = component;
    printf("%d ", vertex);
    ptr_to_edge w = graph->reverse_edges[vertex]->next;
    while (w != NULL)
    {
        if (graph->vertexs[w->to_vertex].visited == 0)
            find_strong_component_in(graph, w->to_vertex, component);
        w = w->next;
    }
    ++graph->counter;
}

void find_strong_component(ptr_to_graph graph)
{
    // initialize graph->counter, graph->vertexs[i].visited
    graph->counter = 0;
    for (int i = 0; i < graph->vertex_num; i++)
        graph->vertexs[i].visited = 0;
    int component = 0;
    while (graph->counter < graph->vertex_num)
    {
        int start_max = -1;
        int max_num = -1;
        for (int i = 0; i < graph->vertex_num; i++)
            if (graph->vertexs[i].visited == 0 && max_num < graph->vertexs[i].num)
            {
                max_num = graph->vertexs[i].num;
                start_max = i;
            }
        
        printf("\ncomponent %d: ", component);
        find_strong_component_in(graph, start_max, ++component);
    }
}

int main()
{
    int vertex_num = 10;
    int edge_num = 15;
    int edges[15][2] = {{0, 3}, {0, 1}, {1, 5}, {1, 2}, {2, 4}, {2, 3}, {2, 0}, 
            {3, 4}, {5, 2}, {6, 7}, {6, 5}, {7, 9}, {7, 5}, {8, 7}, {9, 8}};
    // int edges[15][2] = {{0, 2}, {1, 0}, {2, 5}, {2, 1}, {3, 2}, {3, 0}, {4, 3}, 
    //         {4, 2}, {5, 9}, {5, 6}, {5, 1}, {6, 9}, {6, 7}, {7, 8}, {8, 6}};
    ptr_to_graph graph = create_graph(vertex_num, edges, edge_num);
    assign_num(graph);
    print_graph(graph);
    find_strong_component(graph);
    free_graph(graph);
    printf("\n");
}

