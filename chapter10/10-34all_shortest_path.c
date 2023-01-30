#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#define MaxValue FLT_MAX
#define NotAVertex -1

void create_graph(int vertex_num, int edges[][3], int edge_num, float graph[][vertex_num])
{
    for (int i = 0; i < vertex_num; i++)
    {
        for (int j = 0; j < vertex_num; j++)
            if (i == j)
                graph[i][j] = 0.0f;
            else
                graph[i][j] = MaxValue;
    }

    // read edges
    for (int i = 0; i < edge_num; i++)
    {
        graph[edges[i][0] - 1][edges[i][1] - 1] = edges[i][2];
    }
}

void print_graph(int vertex_num, float graph[][vertex_num])
{
    for (int i = 0; i < vertex_num; i++)
    {
        for (int j = 0; j < vertex_num; j++)
            printf("\nfrom %d to %d: %f", i, j, graph[i][j]);
    }
}

void all_pairs(int vertex_num, float graph[][vertex_num], float dist[][vertex_num], 
                int path[][vertex_num])
{
    int i, j, k;

    /*initialize dist and path */
    for (i =0; i < vertex_num; i++)
        for (int j = 0; j < vertex_num; j++)
        {
            dist[i][j] = graph[i][j];
            path[i][j] = NotAVertex;
        }
    
    for (k = 0; k < vertex_num; k++)
        /* consider each vertex as an intermediate */
        for (i = 0; i < vertex_num; i++)
            for (j = 0; j < vertex_num; j++)
                if (dist[i][k] + dist[k][j] < dist[i][j])
                {
                    /* update shortest path */
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = k;
                }
}

void print_path(int vertex_num, float dist[][vertex_num], int path[][vertex_num])
{

    for (int i = 0; i < vertex_num; i++)
            for (int j = 0; j < vertex_num; j++)
                printf("\nfrom: %d, to: %d, path: %d, dist: %f", i, j, path[i][j], dist[i][j]);

}

int main()
{
    int vertex_num = 7;
    int edge_num = 12;
    int edges[12][3] = {{1, 2, 2}, {1, 4, 1}, {2, 4, 3}, {2, 5, 10}, {3, 1, 4}, {3, 6, 5}, 
            {4, 3, 2}, {4, 5, 2}, {4, 6, 8}, {4, 7, 4}, {5, 7, 6}, {7, 6, 1}};
    float graph[vertex_num][vertex_num];
    create_graph(vertex_num, edges, edge_num, graph);
    print_graph(vertex_num, graph);

    float dist[vertex_num][vertex_num];
    int path[vertex_num][vertex_num];
    all_pairs(vertex_num, graph, dist, path);
    print_path(vertex_num, dist, path);

    printf("\n");
    return 0;
}