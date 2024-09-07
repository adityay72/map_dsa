#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Node
{
    int data;
    int distance;
    struct Node *next;
};

struct Graph
{
    int V;
    struct Node **adjList;
};

// Function to create a new node
struct Node *createNode(int data, int distance)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (!newNode)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->distance = distance;
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

// Function to add an edge to the graph
void addEdge(struct Graph *graph, int src, int dest, int distance)
{
    if (src >= 0 && dest >= 0)
    {
        if (src >= graph->V || dest >= graph->V)
        {
            // Resize the adjacency list if necessary
            int newV = (src > dest ? src : dest) + 1;
            graph->adjList = (struct Node **)realloc(graph->adjList, newV * sizeof(struct Node *));
            if (!graph->adjList)
            {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            for (int i = graph->V; i < newV; i++)
            {
                graph->adjList[i] = NULL;
            }
            graph->V = newV;
        }

        struct Node *newNode = createNode(dest, distance);
        newNode->next = graph->adjList[src];
        graph->adjList[src] = newNode;

        newNode = createNode(src, distance);
        newNode->next = graph->adjList[dest];
        graph->adjList[dest] = newNode;
    }
    else
    {
        printf("Invalid input for nodes.\n");
    }
}

// Function to print the graph
void printGraph(struct Graph *graph)
{
    for (int i = 0; i < graph->V; i++)
    {
        struct Node *temp = graph->adjList[i];
        printf("Adjacency list of vertex %d: ", i);
        while (temp)
        {
            printf("%d (%d) -> ", temp->data, temp->distance);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

// Function to save the map to a file
void saveMapToFile(struct Graph *graph, const char *filename)
{
    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Failed to open the file for writing");
        return;
    }

    // Save the map data to the file
    fprintf(file, "%d\n", graph->V);
    for (int i = 0; i < graph->V; i++)
    {
        struct Node *temp = graph->adjList[i];
        while (temp)
        {
            fprintf(file, "%d %d %d\n", i, temp->data, temp->distance);
            temp = temp->next;
        }
    }

    // Close the file
    fclose(file);
}

int main()
{
    int V = 10; // Initial number of vertices, you can change this
    struct Graph *graph = createGraph(V);

    printf("Welcome to the Map Creator!\n");

    while (true)
    {
        printf("\nMenu:\n");
        printf("1. Add an edge\n");
        printf("2. Save map to file\n");
        printf("3. Print graph\n");
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
                int src, dest, distance; // Declare these variables here
                printf("Enter edge (source, destination, and distance): ");
                if (scanf("%d %d %d", &src, &dest, &distance) != 3)
                {
                    printf("Invalid input for the edge.\n");
                    continue;
                }
                addEdge(graph, src, dest, distance);
                break;
            }

        case 2:
            {
                printf("Enter the filename to save the map: ");
                char filename[256];
                scanf("%255s", filename);
                saveMapToFile(graph, filename);
                break;
            }

        case 3:
            {
                printf("\nMap representation:\n");
                printGraph(graph);
                break;
            }

        case 4:
            {
                printf("Exiting the Map Creator. Goodbye!\n");
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
