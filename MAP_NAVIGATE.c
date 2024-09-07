#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum Direction {
    RIGHT,
    LEFT,
    STRAIGHT,
    BACK
};

struct Node
{
    int data;
    int distance;
    enum Direction direction;
    struct Node *next;
};


struct Graph
{
    int V;
    struct Node **adjList;
    int *minDistance;
};


struct Node *createNode(int data, int distance, enum Direction direction)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (!newNode)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->distance = distance;
    newNode->direction = direction;
    newNode->next = NULL;
    return newNode;
}


struct Graph *createGraph(int V)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    if (!graph)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    graph->V = V;
    graph->adjList = (struct Node **)malloc(V * sizeof(struct Node *));
    graph->minDistance = (int *)malloc(V * sizeof(int));
    if (!graph->adjList || !graph->minDistance)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < V; i++)
    {
        graph->adjList[i] = NULL;
        graph->minDistance[i] = 100000; 
    }
    return graph;
}


void DFS(struct Graph *graph, int src, int dest, bool visited[], int path[], int totalDistance, int pathIndex)
{
    visited[src] = true;
    path[pathIndex] = src;
    pathIndex++;

    if (src == dest)
    {
        printf("Path: ");
        for (int i = 0; i < pathIndex; i++)
        {
            printf("%d", path[i]);
            if (i < pathIndex - 1)
            {
                struct Node *edge = graph->adjList[path[i]];
                while (edge->data != path[i + 1])
                {
                    edge = edge->next;
                }
                char *directionStr;
                switch (edge->direction) {
                    case RIGHT:
                        directionStr = "right";
                        break;
                    case LEFT:
                        directionStr = "left";
                        break;
                    case STRAIGHT:
                        directionStr = "straight";
                        break;
                    case BACK:
                        directionStr = "back";
                        break;
                }
                printf(" (%s) -> ", directionStr);
            }
        }
        printf("\n");

        printf("Total Distance: %d\n", totalDistance);
    }
    else
    {
        struct Node *temp = graph->adjList[src];
        while (temp)
        {
            int neighbor = temp->data;
            if (!visited[neighbor])
            {
                int edgeDistance = temp->distance;
                totalDistance += edgeDistance;
                DFS(graph, neighbor, dest, visited, path, totalDistance, pathIndex);
                totalDistance -= edgeDistance;
            }
            temp = temp->next;
        }
    }

    visited[src] = false; 
}



void findPaths(struct Graph *graph, int src, int dest)
{
    if (src < 0 || src >= graph->V || dest < 0 || dest >= graph->V)
    {
        printf("Invalid source or destination node.\n");
        return;
    }

    bool *visited = (bool *)malloc(graph->V * sizeof(bool));
    if (!visited)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int *path = (int *)malloc(graph->V * sizeof(int));
    if (!path)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < graph->V; i++)
    {
        visited[i] = false;
    }

    printf("Paths from node %d to node %d:\n", src, dest);
    DFS(graph, src, dest, visited, path, 0, 0);

    free(visited);
    free(path);
}


struct Graph *loadMapFromFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Failed to open the file for reading");
        return NULL;
    }

    int V;
    if (fscanf(file, "%d", &V) != 1)
    {
        perror("Invalid file format");
        fclose(file);
        return NULL;
    }

    struct Graph *graph = createGraph(V);
    int src, dest, distance;
    char directionStr[10];
    while (fscanf(file, "%d %d %d %s", &src, &dest, &distance, directionStr) == 4)
    {
        enum Direction direction;
        if (strcmp(directionStr, "right") == 0) {
            direction = RIGHT;
        } else if (strcmp(directionStr, "left") == 0) {
            direction = LEFT;
        } else if (strcmp(directionStr, "straight") == 0) {
            direction = STRAIGHT;
        } else if (strcmp(directionStr, "back") == 0) {
            direction = BACK;
        } else {
            printf("Invalid input for the direction. Please enter 'right', 'left', 'straight', or 'back'.\n");
            continue;
        }

        if (src >= 0 && src < graph->V && dest >= 0 && dest < graph->V)
        {
            struct Node *newNode = createNode(dest, distance, direction);
            newNode->next = graph->adjList[src];
            graph->adjList[src] = newNode;

        }
    }

    fclose(file);
    return graph;
}


int minDistance(struct Graph *graph, bool visited[])
{
    int min = 10000, min_index;
    for (int v = 0; v < graph->V; v++)
    {
        if (!visited[v] && graph->minDistance[v] < min)
        {
            min = graph->minDistance[v];
            min_index = v;
        }
    }
    return min_index;
}


void printShortestPath(struct Graph *graph, int parent[], int dest)
{
    if (parent[dest] == -1)
    {
        printf("%d ", dest);
        return;
    }

    printShortestPath(graph, parent, parent[dest]);
    printf("%d ", dest);
}


void dijkstra(struct Graph *graph, int src)
{
    int parent[graph->V];
    bool visited[graph->V];

    for (int i = 0; i < graph->V; i++)
    {
        parent[i] = -1;
        graph->minDistance[i] = 10000;
        visited[i] = false;
    }

    graph->minDistance[src] = 0;

    for (int count = 0; count < graph->V - 1; count++)
    {
        int u = minDistance(graph, visited);
        visited[u] = true;

        struct Node *temp = graph->adjList[u];
        while (temp != NULL)
        {
            int v = temp->data;
            int weight = temp->distance;

            if (!visited[v] && graph->minDistance[u] != 10000 && graph->minDistance[u] + weight < graph->minDistance[v])
            {
                graph->minDistance[v] = graph->minDistance[u] + weight;
                parent[v] = u;
            }

            temp = temp->next;
        }
    }

    printf("Shortest paths from node %d:\n", src);
    for (int i = 0; i < graph->V; i++)
    {
        if (i != src)
        {
            printf("Path from %d to %d: ", src, i);
            printShortestPath(graph, parent, i);
            printf(" (Distance: %d)\n", graph->minDistance[i]);
        }
    }
}

int main()
{
    printf("Welcome to the Map Navigator!\n");

    printf("Enter the filename to load the map: ");
    char filename[256];
    scanf("%255s", filename);

    struct Graph *graph = loadMapFromFile(filename);

    if (!graph)
    {
        printf("Failed to load the map. Exiting.\n");
        return 1;
    }

    while (true)
    {
        printf("\nMenu:\n");
        printf("1. Find paths\n");
        printf("2. Print map\n");
        printf("3. Find shortest distance\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        int choice;
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a valid option.\n");
            continue;
        }

        switch (choice)
        {
        case 1:
{
    int src, dest;
    printf("Enter the source and destination nodes to find paths: ");
    if (scanf("%d %d", &src, &dest) != 2)
    {
        printf("Invalid input for source and destination nodes.\n");
        continue;
    }
    findPaths(graph, src, dest);
    break;
}


        case 2:
    {
        printf("\nMap representation:\n");
        for (int i = 0; i < graph->V; i++)
        {
            struct Node *temp = graph->adjList[i];
            if (temp != NULL) 
            {
                printf("Adjacency list of vertex %d: ", i);
                while (temp)
                {
                    char *directionStr;
                    switch (temp->direction) {
                        case RIGHT:
                            directionStr = "right";
                            break;
                        case LEFT:
                            directionStr = "left";
                            break;
                        case STRAIGHT:
                            directionStr = "straight";
                            break;
                        case BACK:
                            directionStr = "back";
                            break;
                    }
                    printf("%d (%d, %s)", temp->data, temp->distance, directionStr);
                    temp = temp->next;
                    if (temp != NULL)
                    {
                        printf(" -> ");
                    }
                }
                printf("\n");
            }
        }
        break;
    }


        case 4:
            {
                printf("Exiting the Map Navigator. Goodbye!\n");
                for (int i = 0; i < graph->V; i++)
                {
                    struct Node *temp = graph->adjList[i];
                    while (temp)
                    {
                        struct Node *next = temp->next;
                        free(temp);
                        temp = next;
                    }
                }
                free(graph->adjList);
                free(graph);
                return 0;
            }

        case 3:
        {
            int src;
            printf("Enter the Vertex from which you have to find shortest path to other vertices : ");
            scanf("%d",&src);
            dijkstra(graph,src);
            break;
        }

        default:
            {
                printf("Invalid choice. Please enter a valid option.\n");
            }
        }
    }
}
