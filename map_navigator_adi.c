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
};

// Function to create a new node
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
    newNode->direction = direction; // Set the direction field
    newNode->next = NULL;
    return newNode;
}


// Function to create a new graph
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
    if (!graph->adjList)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < V; i++)
    {
        graph->adjList[i] = NULL;
    }
    return graph;
}

// Function to perform depth-first search
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

    visited[src] = false; // Backtrack
}


// Function to find paths between two nodes
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

// Function to load the map from a file
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

            // For the reverse edge, we'll need to determine the opposite direction
            enum Direction reverseDirection;
            switch (direction) {
                case RIGHT:
                    reverseDirection = LEFT;
                    break;
                case LEFT:
                    reverseDirection = RIGHT;
                    break;
                case STRAIGHT:
                    reverseDirection = BACK;
                    break;
                case BACK:
                    reverseDirection = STRAIGHT;
                    break;
            }

            newNode = createNode(src, distance, reverseDirection);
            newNode->next = graph->adjList[dest];
            graph->adjList[dest] = newNode;
        }
    }

    fclose(file);
    return graph;
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
        printf("3. Exit\n");
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
            if (temp != NULL) // Only print vertices that have adjacent vertices
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


        case 3:
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

        default:
            {
                printf("Invalid choice. Please enter a valid option.\n");
            }
        }
    }
}
