#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define GRID_SIZE 5

#define OCCUPIED 1

#define EMPTY 0

struct coordinate {
    int x;
    int y;
};

// Definir movimientos posibles
struct coordinate left = { -1, 0 };

struct coordinate right = { 1, 0 };

struct coordinate down = { 0, 1 };

struct coordinate up = { 0, -1 };

// Mostrar el entorno del agente
void showGrid(int grid[GRID_SIZE][GRID_SIZE], struct coordinate agent, struct coordinate goal);

int isEqual(struct coordinate, struct coordinate);

int canMoveTo(struct coordinate, int grid[GRID_SIZE][GRID_SIZE]);

void clearScreen() {
  #ifdef _WIN32
      system("cls");  // Comando para Windows
  #else
      system("clear"); // Comando para Linux y macOS
  #endif
}

int main(int argc, char const *argv[]) {
  srand(time(0)); // Inicializar el generador de números aleatorios

  // Entorno de la aspiradora
  int grid[GRID_SIZE][GRID_SIZE] = {
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }
  };

  // Posicionar obstáculos
  grid[1][1] = 1;
  grid[3][2] = 1;

  struct coordinate possibleMoves[4] = { left, right, down, up };

  // Posición inicial del agente
  struct coordinate agentPosition = { 0, 0 };

  // Meta del agente
  struct coordinate goalPosition = { 4, 4 };

  // Mostrar estado inicial del entorno
  showGrid(grid, agentPosition, goalPosition);

  // Mover al agente hasta quedar atrapado o alcanzar la meta
  while(!isEqual(agentPosition, goalPosition)) {
    struct coordinate validMoves[4] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};  // Inicializa las posibles posiciones válidas
    
    int validMoveCount = 0;

    for (size_t i = 0; i < 4; i++) {
      struct coordinate action = possibleMoves[i];

      struct coordinate possibleNewPosition;

      // Calcula la nueva posición
      possibleNewPosition.x = agentPosition.x + action.x;
      
      possibleNewPosition.y = agentPosition.y + action.y;

      // Verifica si se puede mover a la nueva posición
      if (canMoveTo(possibleNewPosition, grid)) {  
        validMoves[validMoveCount++] = possibleNewPosition;
      }
    }

    //  Decisión (elegir un movimiento aleatorio válido)
    if (validMoveCount > 0) {
      int randomIndex = rand() % validMoveCount;
      
      agentPosition = validMoves[randomIndex];
      
      clearScreen();

      showGrid(grid, agentPosition, goalPosition); // Mostrar la nueva posición

      sleep(1);
    } else {
      printf("El agente está atrapado, no hay movimientos válidos.\n");
      
      break;
    }
  }

  // Verificar si el agente llegó a la meta
  if(isEqual(agentPosition, goalPosition)) {
    printf("El agente alcanzo su objetivo \n");
  } else {
    printf("El agente no pudo llegar a su objetivo");
  }

  return 0;
}

// Verifica si se puede mover a la nueva posición
int canMoveTo(struct coordinate nextMove, int grid[GRID_SIZE][GRID_SIZE]) {
  if (nextMove.x >= 0 && nextMove.x < GRID_SIZE && nextMove.y >= 0 && nextMove.y < GRID_SIZE) {

    if (grid[nextMove.x][nextMove.y] != OCCUPIED) {
    
      return 1; // Puede moverse
    }
  }

  return 0; // No puede moverse
}

// Compara si dos coordenadas son iguales
int isEqual(struct coordinate agent, struct coordinate goal) {
  if (agent.x == goal.x && agent.y == goal.y) {
    return 1; // Son iguales
  }

  return 0; // No son iguales
}

// Mostrar el grid con el agente y la meta
void showGrid(int grid[GRID_SIZE][GRID_SIZE], struct coordinate agent, struct coordinate goal) {
  for (size_t i = 0; i < GRID_SIZE; i++) {
    for (size_t y = 0; y < GRID_SIZE; y++) {
      int currentCell = grid[i][y];

      // Si es la posición del agente, imprime el agente con color verde
      if (i == agent.x && y == agent.y) {
        printf("\033[0;32m%2c \033[0m", 'o');
      }
      // Si es la posición de la meta, imprime la meta con color amarillo
      else if (i == goal.x && y == goal.y) {
        printf("\033[0;33m%2s \033[0m", "-");
      }
      // Si la celda está ocupada, imprímela en rojo
      else if (currentCell == OCCUPIED) {
        printf("\033[0;31m%2d \033[0m", currentCell);
      }
      // Si no está ocupada, imprímela normalmente
      else {
        printf("%2d ", currentCell);
      }
    }

    printf("\n");
  }
}
