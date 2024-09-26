#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INF 9999
#define MAX_NODES 6
#define MAP_SIZE 10

typedef struct {
  int x;
  int y;
} Coordinate;

typedef struct node {
  Coordinate coordinate;
  struct node *parent;
  int g;
  int h;
  int f;
} Node;

double euclideanDistance(Coordinate node, Coordinate target);
Node *createNode(Coordinate position, Node *parent, int g, int h);
void aStar(int map[MAX_NODES][MAX_NODES], Coordinate coordinates[MAX_NODES], Coordinate start, Coordinate target);
Node *getLowestNodeFromList(Node *node[], int listSize);
int isSameCoordinate(Coordinate a, Coordinate b);
int isNodeInList(Node *node, Node *list[], int listSize);
void printPath(Node *node);
void printMap(int map[MAX_NODES][MAX_NODES], Coordinate coordinates[MAX_NODES], Node *path);
void printFoundPath(Node *node);

int main(int argc, char const *argv[]) {
  // Matriz de adyacencia: valores representan el costo de las aristas.
  int A[MAX_NODES][MAX_NODES] = {
      {0, 1, 4, INF, INF, INF},
      {1, 0, 2, 6, INF, INF},
      {4, 2, 0, 3, 3, INF},
      {INF, 6, 3, 0, 2, 5},
      {INF, INF, 3, 2, 0, 2},
      {INF, INF, INF, 5, 2, 0}
  };

  // Coordenadas (x, y) para cada nodo
  Coordinate coordinates[MAX_NODES] = {
      {0, 0},  // Nodo 0
      {1, 1},  // Nodo 1
      {2, 2},  // Nodo 2
      {3, 2},  // Nodo 3
      {4, 3},  // Nodo 4
      {5, 3}   // Nodo 5
  };

  Coordinate start = {0, 0};  // Coordenada del nodo inicial
  Coordinate target = {4, 3}; // Coordenada del nodo objetivo

  // Imprime el mapa inicial
  // Ejecuta A* y encuentra el camino
  aStar(A, coordinates, start, target);

  return 0;
}

void aStar(int map[MAX_NODES][MAX_NODES], Coordinate coordinates[MAX_NODES], Coordinate start, Coordinate target) {
  Node *openList[1000];
  Node *closedList[1000];
  int openListLen = 0;
  int closedListLen = 0;

  Node *startNode = createNode(start, NULL, 0, euclideanDistance(start, target));
  openList[openListLen++] = startNode;

  while (openListLen > 0) {
    Node *current = getLowestNodeFromList(openList, openListLen);

    if (isSameCoordinate(current->coordinate, target)) {
      printf("CAMINO ENCONTRADO:\n");
      printFoundPath(current);  // Imprime el camino encontrado
      return;
    }

    for (int i = 0; i < openListLen; i++) {
      if (isSameCoordinate(openList[i]->coordinate, current->coordinate)) {
        openList[i] = openList[--openListLen];
        break;
      }
    }

    closedList[closedListLen++] = current;

    for (int neighborNode = 0; neighborNode < MAX_NODES; neighborNode++) {
      if (map[current->coordinate.y][neighborNode] == INF || neighborNode == current->coordinate.y) {
        continue;
      }

      int gCostForNeighbor = current->g + map[current->coordinate.y][neighborNode];
      
      int hCostForNeighbor = euclideanDistance(coordinates[neighborNode], target);
      
      Node *neighbor = createNode(coordinates[neighborNode], current, gCostForNeighbor, hCostForNeighbor);

      if (isNodeInList(neighbor, closedList, closedListLen)) {
        free(neighbor);
        continue;
      }

      if (!isNodeInList(neighbor, openList, openListLen)) {
        openList[openListLen++] = neighbor;
      } else {
        for (int j = 0; j < openListLen; j++) {
          if (isSameCoordinate(openList[j]->coordinate, neighbor->coordinate) && gCostForNeighbor < openList[j]->g) {
            openList[j]->g = gCostForNeighbor;
            openList[j]->f = gCostForNeighbor + openList[j]->h;
            openList[j]->parent = current;
            free(neighbor);
            break;
          }
        }
      }
    }
  }

  printf("NO SE PUDO ENCONTRAR CAMINO\n");
}

int isSameCoordinate(Coordinate a, Coordinate b) {
  return a.x == b.x && a.y == b.y;
}

int isNodeInList(Node *node, Node *list[], int listSize) {
  for (int i = 0; i < listSize; i++) {
    if (list[i] != NULL && isSameCoordinate(node->coordinate, list[i]->coordinate)) {
      return 1;
    }
  }
  return 0;
}

Node *getLowestNodeFromList(Node *list[], int listSize) {
  Node *lowestNode = NULL;
  for (int i = 0; i < listSize; i++) {
    if (list[i] != NULL && (lowestNode == NULL || list[i]->f < lowestNode->f))
      lowestNode = list[i];
  }
  return lowestNode;
}

double euclideanDistance(Coordinate node, Coordinate target) {
  return sqrt(pow((double)(node.x - target.x), 2) + pow((double)(node.y - target.y), 2));
}

Node *createNode(Coordinate position, Node *parent, int g, int h) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->coordinate = position;
  newNode->parent = parent;
  newNode->g = g;
  newNode->h = h;
  newNode->f = g + h;
  return newNode;
}

void printPath(Node *node) {
  if (node == NULL) return;
  printPath(node->parent);
  printf("Nodo: (%d, %d)\n", node->coordinate.x, node->coordinate.y);
}

void printMap(int map[MAX_NODES][MAX_NODES], Coordinate coordinates[MAX_NODES], Node *path) {
  char displayMap[MAP_SIZE][MAP_SIZE];

  // Inicializar el mapa con '.'
  for (int i = 0; i < MAP_SIZE; i++) {
    for (int j = 0; j < MAP_SIZE; j++) {
      displayMap[i][j] = '.';
    }
  }

  // Colocar nodos en el mapa
  for (int i = 0; i < MAX_NODES; i++) {
    int x = coordinates[i].x;
    int y = coordinates[i].y;
    displayMap[y][x] = 'O';
  }

  // Marcar la ruta en el mapa si se encuentra
  while (path != NULL) {
    displayMap[path->coordinate.y][path->coordinate.x] = 'X';
    path = path->parent;
  }

  // Imprimir el mapa
  for (int i = 0; i < MAP_SIZE; i++) {
    for (int j = 0; j < MAP_SIZE; j++) {
      printf("%c ", displayMap[i][j]);
    }
    printf("\n");
  }
}

// Nueva función para imprimir el camino encontrado
void printFoundPath(Node *node) {
  Node *path[100];  // Almacena el camino
  int pathLen = 0;

  // Recorrer el camino hasta el nodo inicial
  while (node != NULL) {
    path[pathLen++] = node;
    node = node->parent;
  }

  // Imprimir el camino en orden inverso
  for (int i = pathLen - 1; i >= 0; i--) {
    printf("(%d, %d)", path[i]->coordinate.x, path[i]->coordinate.y);
    if (i > 0) {
      printf(" -> ");
    }
  }
  printf("\n");
}
