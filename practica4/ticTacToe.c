#include <stdio.h>

#define CELL_FREE 0
#define CELL_X 1
#define CELL_O -1
#define LEN 3

#define TURN_X 1
#define TURN_O -1

#define WIN_X 1
#define WIN_O -1
#define DRAW 0

typedef int turn;

void minMaxTicTacToe(int board[LEN][LEN], turn player);
int minMax(int board[LEN][LEN], turn player);
int checkBoardForWinningLines(int board[LEN][LEN]);
void printBoard(int board[LEN][LEN]);

int main(int argc, char const *argv[])
{
  int board[LEN][LEN] = {
      {0, 0, 0},
      {0, 0, 0},
      {0, 0, 0}};

  // Empieza el juego con el jugador X
  minMaxTicTacToe(board, TURN_O);

  return 0;
}

void minMaxTicTacToe(int board[LEN][LEN], turn player)
{
  int result = checkBoardForWinningLines(board);

  // Si ya hay un ganador o es empate, terminamos el juego
  if (result != 0)
  {
    if (result == WIN_X)
    {
      printf("¡El jugador X gana!\n");
    }
    else if (result == WIN_O)
    {
      printf("¡El jugador O gana!\n");
    }
    else
    {
      printf("Es un empate.\n");
    }
    return;
  }

  int bestValueMove = (player == TURN_X) ? -1000 : 1000;
  int bestMoveRow = -1;
  int bestMoveCol = -1;

  for (int i = 0; i < LEN; i++)
  {
    for (int j = 0; j < LEN; j++)
    {
      if (board[i][j] == CELL_FREE)
      {
        // Hacer el movimiento
        board[i][j] = player;

        // Ejecutar minimax para encontrar el valor del movimiento
        int moveValue = minMax(board, -player);

        // Deshacer el movimiento
        board[i][j] = CELL_FREE;

        // Actualiza el mejor movimiento para X o el peor para O
        if (player == TURN_X)
        {
          if (moveValue > bestValueMove)
          {
            bestMoveRow = i;
            bestMoveCol = j;
            bestValueMove = moveValue;
          }
        }
        else
        {
          if (moveValue < bestValueMove)
          {
            bestMoveRow = i;
            bestMoveCol = j;
            bestValueMove = moveValue;
          }
        }
      }
    }
  }

  // Realiza el mejor movimiento encontrado
  board[bestMoveRow][bestMoveCol] = player;

  // Imprimir el tablero después del mejor movimiento
  printf("El jugador %c hace un movimiento en (%d, %d):\n", (player == TURN_X) ? 'X' : 'O', bestMoveRow + 1, bestMoveCol + 1);
  printBoard(board);

  // Cambia el turno al otro jugador y sigue jugando
  minMaxTicTacToe(board, -player);
}

int minMax(int board[LEN][LEN], turn player)
{
  int winner = checkBoardForWinningLines(board);
  if (winner == WIN_X)
    return WIN_X;
  if (winner == WIN_O)
    return WIN_O;
  if (winner == DRAW)
    return DRAW;

  int bestValue = (player == TURN_X) ? -1000 : 1000;

  for (int i = 0; i < LEN; i++)
  {
    for (int j = 0; j < LEN; j++)
    {
      if (board[i][j] == CELL_FREE)
      {
        board[i][j] = player;

        int moveValue = minMax(board, -player);

        board[i][j] = CELL_FREE;

        if (player == TURN_X)
        {
          bestValue = (moveValue > bestValue) ? moveValue : bestValue;
        }
        else
        {
          bestValue = (moveValue < bestValue) ? moveValue : bestValue;
        }
      }
    }
  }

  return bestValue;
}

int checkBoardForWinningLines(int board[LEN][LEN])
{
  // Comprobar filas y columnas
  for (int i = 0; i < LEN; i++)
  {
    if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != CELL_FREE)
    {
      return (board[i][0] == CELL_X) ? WIN_X : WIN_O;
    }
    if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != CELL_FREE)
    {
      return (board[0][i] == CELL_X) ? WIN_X : WIN_O;
    }
  }

  // Comprobar diagonales
  if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != CELL_FREE)
  {
    return (board[0][0] == CELL_X) ? WIN_X : WIN_O;
  }
  if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != CELL_FREE)
  {
    return (board[0][2] == CELL_X) ? WIN_X : WIN_O;
  }

  // Comprobar si hay empate (tablero lleno)
  for (int i = 0; i < LEN; i++)
  {
    for (int j = 0; j < LEN; j++)
    {
      if (board[i][j] == CELL_FREE)
      {
        return 0; // El juego sigue
      }
    }
  }

  return DRAW; // Empate
}

void printBoard(int board[LEN][LEN])
{
  for (int i = 0; i < LEN; i++)
  {
    for (int j = 0; j < LEN; j++)
    {
      if (board[i][j] == CELL_X)
        printf("X ");
      else if (board[i][j] == CELL_O)
        printf("O ");
      else
        printf("- ");
    }
    printf("\n");
  }
}
