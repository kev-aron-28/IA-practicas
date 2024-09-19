#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define OCCUPIED 1

#define GRID_SIZE 10

#define MAX_NEIGHBORS 8

typedef struct
{
  int x;
  int y;
} Coordinate;

typedef struct node
{
  Coordinate coordinate;
  struct Node *parent;
  int g;
  int h;
  int f;
} Node;

int manhattanDistance(Coordinate node, Coordinate target);

void printMap(Coordinate currentPosition, Coordinate goal, int grid[10][10]);

Node *createNode(Coordinate position, Node *parent, int g, int h);

void aStar(int map[10][10], Coordinate start, Coordinate target);

Node *getLowestNodeFromList(Node *node[], int listSize);

int isSameCoordinate(Coordinate a, Coordinate b);

int isMovementInsideBoard(Coordinate pos);

int isNodeInList(Node *node, Node *list[], int listSize);

void pathToTarget(Node *node, int map[GRID_SIZE][GRID_SIZE]);

int main(int argc, char const *argv[])
{
  int map[10][10] = {
      {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
      {0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 1, 0, 1, 1, 1, 1, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
  };

  Coordinate start = {0, 0};

  Coordinate target = {9, 9};

  printMap(start, target, map);

  aStar(map, start, target);

  return 0;
}

void aStar(int map[10][10], Coordinate start, Coordinate target)
{
  Node *openList[1000];
  
  Node *closedList[1000];
  
  int openListLen = 0; // Inicializa openListLen en 0

  int closedListLen = 0;

  Node *startNode = createNode(start, NULL, 0, manhattanDistance(start, target));

  openList[openListLen++] = startNode;

  while (openListLen > 0)
  {
    Node * current = getLowestNodeFromList(openList, openListLen);

    if (isSameCoordinate(current->coordinate, target))
    {
      printf("DONE SEARCHING\n");

      pathToTarget(current, map);

      printMap(start, target, map);
      return;
    }

    for (int i = 0; i < openListLen; i++)
    {
      if (isSameCoordinate(openList[i]->coordinate, current->coordinate))
      {
        openList[i] = openList[--openListLen];
        break;
      }
    }

    closedList[closedListLen++] = current;

    Coordinate neighborPositions[MAX_NEIGHBORS] = {
        {current->coordinate.x - 1, current->coordinate.y},     // Up
        {current->coordinate.x + 1, current->coordinate.y},     // Down
        {current->coordinate.x, current->coordinate.y - 1},     // Left
        {current->coordinate.x, current->coordinate.y + 1},     // Right
        {current->coordinate.x - 1, current->coordinate.y - 1}, // Up-Left (Diagonal)
        {current->coordinate.x - 1, current->coordinate.y + 1}, // Up-Right (Diagonal)
        {current->coordinate.x + 1, current->coordinate.y - 1}, // Down-Left (Diagonal)
        {current->coordinate.x + 1, current->coordinate.y + 1}  // Down-Right (Diagonal)
    };

    for (int currentNeiIndx = 0; currentNeiIndx < MAX_NEIGHBORS; currentNeiIndx++)
    {
      Coordinate neighborPositionToTry = neighborPositions[currentNeiIndx];

      if (!isMovementInsideBoard(neighborPositionToTry) || map[neighborPositionToTry.x][neighborPositionToTry.y] == OCCUPIED)
      {
        continue;
      }

      int gCostForNeighbor = current->g + 1;
      int hCostForNeighbor = manhattanDistance(neighborPositionToTry, target); // Corrige la llamada a manhattanDistance
      Node *neighbor = createNode(neighborPositionToTry, current, gCostForNeighbor, hCostForNeighbor);

      if (isNodeInList(neighbor, closedList, closedListLen))
      {
        free(neighbor);
        continue;
      }

      if (!isNodeInList(neighbor, openList, openListLen))
      {
        openList[openListLen++] = neighbor;
      }
      else
      {
        // CHECK IF THIS PATH IS BETTER
        for (int j = 0; j < openListLen; j++)
        {
          if (isSameCoordinate(openList[j]->coordinate, neighborPositionToTry) && gCostForNeighbor < openList[j]->g)
          {
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

  printf("UNABLE TO FIND PATH \n");
}

int isSameCoordinate(Coordinate a, Coordinate b)
{
  if ((a.x == b.x) && (a.y == b.y))
    return 1;

  return 0;
}

int isMovementInsideBoard(Coordinate pos)
{
  return (pos.x >= 0 && pos.x < GRID_SIZE && pos.y >= 0 && pos.y < GRID_SIZE);
}

int isNodeInList(Node *node, Node *list[], int listSize)
{
  for (int i = 0; i < listSize; i++)
  {
    if (list[i] != NULL && isSameCoordinate(node->coordinate, list[i]->coordinate))
    {
      return 1;
    }
  }
  return 0;
}

Node *getLowestNodeFromList(Node *list[], int listSize)
{
  Node *lowestNode = NULL;

  for (size_t i = 0; i < listSize; i++)
  {
    if (list[i] != NULL && (lowestNode == NULL || list[i]->f < lowestNode->f))
      lowestNode = list[i];
  }

  return lowestNode;
}

int manhattanDistance(Coordinate node, Coordinate target)
{
  int h = abs(node.x - target.x) + abs(node.y - target.y);

  return h;
}

Node *createNode(Coordinate position, Node *parent, int g, int h)
{
  Node *newNode = (Node *)malloc(sizeof(Node) * 1);

  newNode->coordinate = position;

  newNode->parent = parent;

  newNode->g = g;

  newNode->h = h;

  newNode->f = g + h;

  return newNode;
}

void printMap(Coordinate currentPosition, Coordinate goal, int grid[10][10])
{
  for (size_t i = 0; i < GRID_SIZE; i++)
  {
    for (size_t y = 0; y < GRID_SIZE; y++)
    {
      int currentCell = grid[i][y];

      if (i == currentPosition.x && y == currentPosition.y)
      {
        printf("\033[0;32m%2c \033[0m", 'o');
      }
      else if (i == goal.x && y == goal.y)
      {
        printf("\033[0;33m%2s \033[0m", "-");
      }
      else if (currentCell == OCCUPIED)
      {
        printf("\033[0;31m%2d \033[0m", currentCell);
      }
      else if (currentCell == 2)
      { // Path marker
        printf("\033[0;34m%2c \033[0m", '*');
      }
      else
      {
        printf("%2d ", currentCell);
      }
    }
    printf("\n");
  }
}

void pathToTarget(Node *node, int map[GRID_SIZE][GRID_SIZE])
{
  while (node != NULL)
  {
    map[node->coordinate.x][node->coordinate.y] = 2;
    node = node->parent;
  }
}
