#include <stdio.h>
#include <stdlib.h>

#define VISITED 1
#define NOTVISITED 0
#define ISADJACENT 1

// Prototipo de la función BFS
void breadthFirstSearch(int **adj, int numNodes, int startNode);

void depthFirstSearch(int **adj, int numNodes, int startNode);

// Función para imprimir el menú y obtener la opción del usuario
int printMenu();

int main() {
    int numNodes = 6;

    // Matriz de adyacencia dinámica
    int **adjacentMatrix = (int **)malloc(numNodes * sizeof(int *));
    for (int i = 0; i < numNodes; i++) {
        adjacentMatrix[i] = (int *)malloc(numNodes * sizeof(int));
    }

    // Inicializar la matriz de adyacencia
    int initMatrix[6][6] = {
        { 0, 1, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1 },
        { 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 0 }
    };

    // Copiar valores iniciales a la matriz dinámica
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            adjacentMatrix[i][j] = initMatrix[i][j];
        }
    }

    int option;
    // Mostrar menú y obtener la opción del usuario
    option = printMenu();

    // Ejecutar la búsqueda en anchura (BFS)
    if(option == 1) {
      depthFirstSearch(adjacentMatrix, numNodes, 0);
    } else {
      breadthFirstSearch(adjacentMatrix, numNodes, 0);
    }

    // Liberar la memoria dinámica
    for (int i = 0; i < numNodes; i++) {
        free(adjacentMatrix[i]);
    }
    free(adjacentMatrix);

    return 0;
}

// Implementación de la BFS
void breadthFirstSearch(int **adj, int numNodes, int startNode) {
    int queueMaxCapacity = 10;
    int *queue = (int *)malloc(sizeof(int) * queueMaxCapacity);
    int *visitedNodes = (int *)calloc(numNodes, sizeof(int)); // Array de nodos visitados
    int rearIndex = 0, frontIndex = 0; // Índices de la cola

    // Inicializar BFS
    visitedNodes[startNode] = VISITED;
    queue[rearIndex++] = startNode;

    printf("Order of visited nodes:\n");

    // Realizar BFS
    while (frontIndex < rearIndex) {
        int currentNode = queue[frontIndex++];

        // Imprimir el nodo actual si se solicita
        printf("Visited Node: %d\n", currentNode);

        // Explorar nodos adyacentes
        for (int i = 0; i < numNodes; i++) {
            if (adj[currentNode][i] == ISADJACENT && visitedNodes[i] == NOTVISITED) {
                // Si la cola está llena, expandir su capacidad
                if (rearIndex == queueMaxCapacity) {
                    queueMaxCapacity *= 2;
                    queue = (int *)realloc(queue, sizeof(int) * queueMaxCapacity);
                }

                // Marcar el nodo como visitado y añadirlo a la cola
                visitedNodes[i] = VISITED;
                queue[rearIndex++] = i;
            }
        }
    }

    // Liberar memoria dinámica
    free(queue);
    free(visitedNodes);
}

void depthFirstSearch(int **adj, int numNodes, int startNode) {
    static int *visitedNodes = NULL; // Array de nodos visitados
    if (visitedNodes == NULL) {
        visitedNodes = (int *)calloc(numNodes, sizeof(int));
    }

    // Marcar el nodo como visitado
    visitedNodes[startNode] = VISITED;

    printf("Visited Node: %d\n", startNode);

    // Explorar nodos adyacentes
    for (int i = 0; i < numNodes; i++) {
        if (adj[startNode][i] == ISADJACENT && visitedNodes[i] == NOTVISITED) {
            depthFirstSearch(adj, numNodes, i);
        }
    }
}

// Función para imprimir el menú y obtener la opción del usuario
int printMenu() {
    int option;

    printf("Menu:\n");
    printf("1. Busqueda en profundidad\n");
    printf("2. Busqueda en anchura\n");
    printf("Elija una opción: ");
    scanf("%d", &option);

    // Validar la opción
    while (option != 1 && option != 2) {
        printf("Opción no válida. Por favor, elija 1 o 2: ");
        scanf("%d", &option);
    }

    return option;
}
