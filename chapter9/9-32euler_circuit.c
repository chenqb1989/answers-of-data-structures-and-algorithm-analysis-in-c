/*
note:
1. define visited in edge, and define reverse in edge, to symbol its reverse visited when needed
2. I do not test situations that combine several circuits, there might be mistaks to deal with that
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MaxVertexNum (100)

/* struct t_node */
struct t_node;
typedef struct t_node *t_list;

struct t_node
{
    int element;
    t_list next;
};

/* struct t_edge */
struct t_edge;
typedef struct t_edge *ptr_to_edge;

struct t_edge
{
    int from_vertex;
    int to_vertex;
    char visited;
    ptr_to_edge reverse_edge;
    ptr_to_edge next;
};

/* struct t_graph */
struct t_graph;
typedef struct t_graph *ptr_to_graph;

struct t_graph
{
    int vertex_num;
    ptr_to_edge *edges;
    ptr_to_edge *last_visited;
    t_list euler_path;
    t_list euler_path_step;
    int path_size;
};

/* ******list func****** */
int insert_list(int x, t_list p)
{
    t_list tmp_cell = malloc(sizeof(struct t_node));
    if (tmp_cell == NULL)
        return -1;
    tmp_cell->element = x;
    tmp_cell->next = p->next;
    p->next = tmp_cell;
    return 0;
}

void init_list(t_list lst)
{
    lst->element = -1;
    lst->next = NULL;
}

void print_list(t_list l)
{
    t_list p = l->next;
    while (p != NULL)
    {
        printf(" %d", p->element);
        p = p->next;
    }
}

int free_list(t_list lst)
{
    t_list del_p;
    while (lst != NULL)
    {
        del_p = lst;
        lst = lst->next;
        free(del_p);
    }
    free(lst);
    return 0;
}

/* ******edge func****** */
int insert_edge(int from_vertex, int to_vertex, ptr_to_edge p)
{
    ptr_to_edge tmp_cell = (ptr_to_edge)malloc(sizeof(struct t_edge));
    if (tmp_cell == NULL)
        return -1;
    tmp_cell->from_vertex = from_vertex;
    tmp_cell->to_vertex = to_vertex;
    tmp_cell->visited = 0;
    tmp_cell->next = p->next;
    p->next = tmp_cell;
    return 0;
}

void init_edge(ptr_to_edge lst)
{
    lst->to_vertex = -1;
    lst->from_vertex = -1;
    lst->visited = 0;
    lst->reverse_edge = lst;
    lst->next = NULL;
}

void print_edges(ptr_to_edge l)
{
    ptr_to_edge p = l->next;
    while (p != NULL)
    {
        printf(" to %d, visited: %d, reverse: %d to %d; ", p->to_vertex, p->visited,
            p->reverse_edge->from_vertex, p->reverse_edge->to_vertex);
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
        
    graph->edges = malloc(MaxVertexNum * sizeof(t_list));
    if (graph->edges == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    graph->last_visited = malloc(MaxVertexNum * sizeof(t_list));
    if (graph->last_visited == NULL)
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
        graph->last_visited[i] = graph->edges[i];
    }

    // read edges
    for (int i = 0; i < edge_num; i++)
    {
        if (insert_edge(edges[i][0], edges[i][1], graph->edges[edges[i][0]]) < 0)
            return NULL;
        if (insert_edge(edges[i][1], edges[i][0], graph->edges[edges[i][1]]) < 0)
            return NULL;
        graph->edges[edges[i][0]]->next->reverse_edge = graph->edges[edges[i][1]]->next;
        graph->edges[edges[i][1]]->next->reverse_edge = graph->edges[edges[i][0]]->next;
    }

    graph->euler_path = malloc(sizeof(struct t_node));
    if (graph->euler_path == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    graph->euler_path_step = malloc(sizeof(struct t_node));
    if (graph->euler_path_step == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    init_list(graph->euler_path);
    init_list(graph->euler_path_step);

    graph->path_size = 0;
    return graph;
}

void print_graph(ptr_to_graph graph)
{
    if (graph != NULL)
        for (int i = 0; i < graph->vertex_num; i++)
        {
            printf("\nvertex: %d, edges: ", i);
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
        free(graph->euler_path);
        free(graph->euler_path_step);
        free(graph);
    }
}

t_list find_start(ptr_to_graph graph)
{
    if (graph->path_size == 0)
    {
        if (insert_list(0, graph->euler_path) < 0)
            return NULL;
        return graph->euler_path->next;
    }
    else
    {
        t_list l = graph->euler_path->next;
        while (l != NULL)
        {
            while (graph->last_visited[l->element]->next != NULL && graph->last_visited[l->element]->next->visited == 1)
            {
                graph->last_visited[l->element] = graph->last_visited[l->element]->next;
            }
                
            if (graph->last_visited[l->element]->next != NULL)
                break;
            l = l->next;
        }
        return l;
    }
}

int find_euler_step(ptr_to_graph graph, int start, int end)
{
    while (graph->last_visited[start]->next && graph->last_visited[start]->next->visited == 1)
    {
        graph->last_visited[start] = graph->last_visited[start]->next;
    }

    if (graph->last_visited[start]->next == NULL)
        return 0;
    
    if (insert_list(start, graph->euler_path_step) < 0)
        return -1;

    
    graph->last_visited[start]->next->visited = 1;
    graph->last_visited[start]->next->reverse_edge->visited = 1;
    graph->last_visited[start] = graph->last_visited[start]->next;
    graph->path_size++;

    if (graph->last_visited[start]->to_vertex == end)
        return 1;
    else
        return find_euler_step(graph, graph->last_visited[start]->to_vertex, end);
}

int euler_circit(ptr_to_graph graph)
{
    t_list start;
    
    while (1)
    {
        start = find_start(graph);
        if (start == NULL)
            break;
        int flag = find_euler_step(graph, start->element, start->element);
        if (flag < 0)
            return -1;
        else if (flag == 0)
        {
            printf("there is no euler circuit!\n");
            return 0;
        }
        else
        {
            t_list start_next = start->next;
            start->next = graph->euler_path_step->next;
            t_list step_end = graph->euler_path_step;

            while (step_end->next != NULL)
                step_end = step_end->next;
            step_end->next = start_next;

            graph->euler_path_step->next = NULL;
        }
    }
}

int main()
{
    int vertex_num = 12;
    int edge_num = 21;
    int edges[21][2] = {{0, 3}, {0, 2}, {1, 2}, {1, 7}, {2, 8}, {2, 6}, {2, 5}, {2, 3},
        {3, 6}, {3, 10}, {3, 9}, {3, 4}, {4, 9}, {5, 8}, {6, 8}, {6, 9}, {7, 8}, {8, 11},
        {8, 9}, {9, 11}, {9, 10}};
    ptr_to_graph graph = create_graph(vertex_num, edges, edge_num);
    print_graph(graph);
    euler_circit(graph);
    printf("result:\n");
    t_list e = graph->euler_path->next;
    while (e != NULL)
    {
        printf("%d ", e->element);
        e = e->next;
    }
        
    free_graph(graph);
    printf("\n");
}

