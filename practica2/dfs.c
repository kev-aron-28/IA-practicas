#include <stdio.h>
#include <stdlib.h>

#define VISITED 1
#define NOTVISITED 0
#define ISADJACENT 1

// Prototipo de la función BFS
void depthFirstSearch(int **adj, int numNodes, int startNode, int showOrder);

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
        { 0, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 0 },
        { 0, 0, 0, 0, 0, 1 },
        { 0, 0, 0, 0, 0, 1 },
        { 0, 0, 0, 0, 0, 0 }
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
    depthFirstSearch(adjacentMatrix, numNodes, 0, option);

    // Liberar la memoria dinámica
    for (int i = 0; i < numNodes; i++) {
        free(adjacentMatrix[i]);
    }
    free(adjacentMatrix);

    return 0;
}

// Implementación de la BFS
void depthFirstSearch(int **adj, int numNodes, int startNode, int showOrder) {
    static int *visitedNodes = NULL; // Array de nodos visitados
    if (visitedNodes == NULL) {
        visitedNodes = (int *)calloc(numNodes, sizeof(int));
    }

    // Marcar el nodo como visitado
    visitedNodes[startNode] = VISITED;

    if (showOrder) {
        printf("Visited Node: %d\n", startNode);
    }

    // Explorar nodos adyacentes
    for (int i = 0; i < numNodes; i++) {
        if (adj[startNode][i] == ISADJACENT && visitedNodes[i] == NOTVISITED) {
            depthFirstSearch(adj, numNodes, i, showOrder);
        }
    }
}

// Función para imprimir el menú y obtener la opción del usuario
int printMenu() {
    int option;

    printf("Menu:\n");
    printf("1. Mostrar orden de nodos visitados\n");
    printf("2. No mostrar orden de nodos visitados\n");
    printf("Elija una opción: ");
    scanf("%d", &option);

    // Validar la opción
    while (option != 1 && option != 2) {
        printf("Opción no válida. Por favor, elija 1 o 2: ");
        scanf("%d", &option);
    }

    return option == 1;
}
