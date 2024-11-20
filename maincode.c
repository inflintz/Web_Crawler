#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_URL_LEN 100
#define MAX_PAGES 15

typedef struct GraphNode {
    int pageIndex;
    struct GraphNode *next;
} GraphNode;

typedef struct Graph {
    int numPages;
    char urls[MAX_PAGES][MAX_URL_LEN];
    GraphNode *adjList[MAX_PAGES];
} Graph;

typedef struct QueueNode {
    int pageIndex;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front, *rear;
} Queue;

GraphNode* createGraphNode(int pageIndex) {
    GraphNode *newNode = (GraphNode *)malloc(sizeof(GraphNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->pageIndex = pageIndex;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int numPages) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (!graph) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    graph->numPages = numPages;
    for (int i = 0; i < numPages; i++) {
        graph->adjList[i] = NULL;
        snprintf(graph->urls[i], MAX_URL_LEN, "Page_%d", i);  // default URL names
    }
    return graph;
}

void setPageUrl(Graph *graph, int pageIndex, const char *url) {
    if (pageIndex < MAX_PAGES) {
        strncpy(graph->urls[pageIndex], url, MAX_URL_LEN);
    }
}

void addEdge(Graph *graph, int src, int dest) {
    GraphNode *newNode = createGraphNode(dest);
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;
}

void freeGraph(Graph *graph) {
    for (int i = 0; i < graph->numPages; i++) {
        GraphNode *current = graph->adjList[i];
        while (current) {
            GraphNode *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph);
}

Queue* createQueue() {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (!q) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, int pageIndex) {
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->pageIndex = pageIndex;
    newNode->next = NULL;
    if (q->rear != NULL) {
        q->rear->next = newNode;
    }
    q->rear = newNode;
    if (q->front == NULL) {
        q->front = newNode;
    }
}

int dequeue(Queue *q) {
    if (q->front == NULL) {
        return -1;
    }
    QueueNode *temp = q->front;
    int pageIndex = temp->pageIndex;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return pageIndex;
}

void freeQueue(Queue *q) {
    while (q->front != NULL) {
        dequeue(q);
    }
    free(q);
}

void printGraph(Graph *graph) {
    for (int i = 0; i < graph->numPages; i++) {
        GraphNode *temp = graph->adjList[i];
        printf("Page %d (%s):", i, graph->urls[i]);
        while (temp) {
            printf(" -> Page %d (%s)", temp->pageIndex, graph->urls[temp->pageIndex]);
            temp = temp->next;
        }
        printf("\n");
    }
}

void bfsTraversal(Graph *graph, int start) {
    int visited[MAX_PAGES] = {0};
    Queue *q = createQueue();

    printf("BFS Traversal starting from page %d (%s):\n", start, graph->urls[start]);
    enqueue(q, start);
    visited[start] = 1;

    while (q->front != NULL) {
        int pageIndex = dequeue(q);
        printf("Visited: Page %d (%s)\n", pageIndex, graph->urls[pageIndex]);

        GraphNode *adjList = graph->adjList[pageIndex];
        while (adjList != NULL) {
            int adjIndex = adjList->pageIndex;
            if (!visited[adjIndex]) {
                visited[adjIndex] = 1;
                enqueue(q, adjIndex);
            }
            adjList = adjList->next;
        }
    }
    freeQueue(q);
}

void dfsTraversal(Graph *graph, int start, int visited[]) {
    visited[start] = 1;
    printf("Visited: Page %d (%s)\n", start, graph->urls[start]);

    GraphNode *adjList = graph->adjList[start];
    while (adjList != NULL) {
        int adjIndex = adjList->pageIndex;
        if (!visited[adjIndex]) {
            dfsTraversal(graph, adjIndex, visited);
        }
        adjList = adjList->next;
    }
}

int main() {
    int numPages = 10;
    Graph *graph = createGraph(numPages);

    setPageUrl(graph, 0, "http://example.com");
    setPageUrl(graph, 1, "http://example.com/page1");
    setPageUrl(graph, 2, "http://example.com/page2");
    setPageUrl(graph, 3, "http://example.com/page1/subpage1");
    setPageUrl(graph, 4, "http://example.com/page1/subpage2");
    setPageUrl(graph, 5, "http://example.com/page2/subpage1");
    setPageUrl(graph, 6, "http://example.com/page2/subpage2");
    setPageUrl(graph, 7, "http://example.com/page1/subpage1/subsubpage1");
    setPageUrl(graph, 8, "http://example.com/page1/subpage2/subsubpage1");
    setPageUrl(graph, 9, "http://example.com/page2/subpage1/subsubpage1");

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 5);
    addEdge(graph, 2, 6);
    addEdge(graph, 3, 7);
    addEdge(graph, 4, 8);
    addEdge(graph, 5, 9);

    printGraph(graph);

    int choice;
    printf("Choose traversal method:\n1. BFS\n2. DFS\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        bfsTraversal(graph, 0);
    } else if (choice == 2) {
        int visited[MAX_PAGES] = {0};
        printf("DFS Traversal starting from page 0:\n");
        dfsTraversal(graph, 0, visited);
    } else {
        printf("Invalid choice!\n");
    }

    freeGraph(graph);

    return 0;
}
