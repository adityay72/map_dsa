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


struct Node *createNode(int data, int distance, enum Direction direction)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->distance = distance;
    newNode->direction = direction;
    newNode->next = NULL;
    return newNode;
}



struct Graph *createGraph(int V)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    
    graph->V = V;
    graph->adjList = (struct Node **)malloc(V * sizeof(struct Node *));
    
    for (int i = 0; i < V; i++)
    {
        graph->adjList[i] = NULL;
    }
    return graph;
}


void addEdge(struct Graph *graph, int src, int dest, int distance, enum Direction direction)
{
    if (src >= 0 && dest >= 0)
    {
        if (src >= graph->V || dest >= graph->V)
        {
            
            int newV = (src > dest ? src : dest) + 1;
            graph->adjList = (struct Node **)realloc(graph->adjList, newV * sizeof(struct Node *));
            
            for (int i = graph->V; i < newV; i++)
            {
                graph->adjList[i] = NULL;
            }
            graph->V = newV;
        }

        struct Node *newNode = createNode(dest, distance, direction);
        newNode->next = graph->adjList[src];
        graph->adjList[src] = newNode;
    }
    else
    {
        printf("Invalid input for nodes.\n");
    }
}



void printGraph(struct Graph *graph)
{
    for (int i = 0; i < graph->V; i++)
    {
        struct Node *temp = graph->adjList[i];
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
            printf("%d (%d, %s) -> ", temp->data, temp->distance, directionStr);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}



void saveMapToFile(struct Graph *graph, const char *filename)
{
    
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Failed to open the file for writing");
        return;
    }

    
    fprintf(file, "%d\n", graph->V);
    for (int i = 0; i < graph->V; i++)
    {
        struct Node *temp = graph->adjList[i];
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
            fprintf(file, "%d %d %d %s\n", i, temp->data, temp->distance, directionStr);
            temp = temp->next;
        }
    }

   
    fclose(file);
}


int main()
{
    int V = 10; 
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
    
    
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    
    continue;
    }


        switch (choice)
        {
        case 1:
    {
        int src, dest, distance;
        enum Direction direction; 
        printf("Enter edge (source, destination, distance, and direction): ");
        char directionInput[10];
        if (scanf("%d %d %d %s", &src, &dest, &distance, directionInput) != 4)
        {
            printf("Invalid input for the edge.\n");
            continue;
        }
        
        if (strcmp(directionInput, "right") == 0) {
            direction = RIGHT;
        } else if (strcmp(directionInput, "left") == 0) {
            direction = LEFT;
        } else if (strcmp(directionInput, "straight") == 0) {
            direction = STRAIGHT;
        } else if (strcmp(directionInput, "back") == 0) {
            direction = BACK;
        } else {
            printf("Invalid input for the direction. Please enter 'right', 'left', 'straight', or 'back'.\n");
            continue;
        }

        addEdge(graph, src, dest, distance, direction);
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
