#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h> // Para sqrt y pow

#define MAX_TIME 48
#define MAX_OBJECTS 10

typedef struct object
{
  char name[50];
  int x, y; // Coordenadas del cliente o centro de distribución
} Object;

typedef struct predicate
{
  char name[50];
  bool (*evaluate)(Object, Object);
} Predicate;

int D(Object client, Object distribution);

int T(Object client, Object distribution);

bool ForAll(Predicate p, Object clients[], int clientCount, Object distributions[], int distCount);

bool ThereExists(Predicate p, Object clients[], int clientCount, Object distributions[], int distCount);

bool IsDeliveryPossible(Object client, Object distribution);

void addClient(Object clients[], int *clientCount);

void addDistribution(Object distributions[], int *distCount);

void showClients(Object clients[], int clientCount);

void showDistributions(Object distributions[], int distCount);

int main(int argc, char const *argv[])
{
  Object clients[MAX_OBJECTS];
  Object distributions[MAX_OBJECTS];
  int clientCount = 0, distCount = 0;
  int choice;

  Predicate delivery_predicate = {"DeliveryPossible", IsDeliveryPossible};

  while (1)
  {
    printf("\n=== Sistema de Entregas ===\n");
    printf("1. Agregar Cliente\n");
    printf("2. Agregar Centro de Distribución\n");
    printf("3. Mostrar Clientes\n");
    printf("4. Mostrar Centros de Distribución\n");
    printf("5. Verificar Entregas Posibles\n");
    printf("6. Salir\n");
    printf("Seleccione una opción: ");
    scanf("%d", &choice);
    getchar(); // Limpiar el buffer

    switch (choice)
    {
    case 1:
      addClient(clients, &clientCount);
      break;
    case 2:
      addDistribution(distributions, &distCount);
      break;
    case 3:
      showClients(clients, clientCount);
      break;
    case 4:
      showDistributions(distributions, distCount);
      break;
    case 5:
      if (ForAll(delivery_predicate, clients, clientCount, distributions, distCount))
      {
        printf("Todos los clientes pueden ser atendidos por al menos un centro de distribución dentro del tiempo límite.\n");
      }
      else
      {
        printf("No todos los clientes pueden ser atendidos dentro del tiempo límite.\n");
      }
      break;
    case 6:
      printf("Saliendo del programa...\n");
      return 0;
    default:
      printf("Opción no válida. Intente de nuevo.\n");
    }
  }

  return 0;
}

// Añadir cliente
void addClient(Object clients[], int *clientCount)
{
  if (*clientCount >= MAX_OBJECTS)
  {
    printf("Número máximo de clientes alcanzado.\n");
    return;
  }

  printf("Ingrese el nombre del cliente: ");
  fgets(clients[*clientCount].name, 50, stdin);
  clients[*clientCount].name[strcspn(clients[*clientCount].name, "\n")] = 0; // Remover salto de línea

  printf("Ingrese la coordenada X del cliente: ");
  scanf("%d", &clients[*clientCount].x);
  printf("Ingrese la coordenada Y del cliente: ");
  scanf("%d", &clients[*clientCount].y);
  getchar(); // Limpiar el buffer

  (*clientCount)++;
  printf("Cliente añadido exitosamente.\n");
}

// Añadir centro de distribución
void addDistribution(Object distributions[], int *distCount)
{
  if (*distCount >= MAX_OBJECTS)
  {
    printf("Número máximo de centros de distribución alcanzado.\n");
    return;
  }

  printf("Ingrese el nombre del centro de distribución: ");
  fgets(distributions[*distCount].name, 50, stdin);
  distributions[*distCount].name[strcspn(distributions[*distCount].name, "\n")] = 0; // Remover salto de línea

  printf("Ingrese la coordenada X del centro de distribución: ");
  scanf("%d", &distributions[*distCount].x);
  printf("Ingrese la coordenada Y del centro de distribución: ");
  scanf("%d", &distributions[*distCount].y);
  getchar(); // Limpiar el buffer

  (*distCount)++;
  printf("Centro de distribución añadido exitosamente.\n");
}

// Mostrar clientes
void showClients(Object clients[], int clientCount)
{
  printf("\n=== Clientes ===\n");
  for (int i = 0; i < clientCount; i++)
  {
    printf("%d. %s (X: %d, Y: %d)\n", i + 1, clients[i].name, clients[i].x, clients[i].y);
  }
}

// Mostrar centros de distribución
void showDistributions(Object distributions[], int distCount)
{
  printf("\n=== Centros de Distribución ===\n");
  for (int i = 0; i < distCount; i++)
  {
    printf("%d. %s (X: %d, Y: %d)\n", i + 1, distributions[i].name, distributions[i].x, distributions[i].y);
  }
}

// Calcular la distancia entre un cliente y un centro de distribución usando la fórmula euclidiana
int D(Object client, Object distribution)
{
  int dx = client.x - distribution.x;
  int dy = client.y - distribution.y;
  return (int)sqrt(pow(dx, 2) + pow(dy, 2)); // Distancia euclidiana
}

// Calcular el tiempo de entrega basado en la distancia
int T(Object client, Object distribution)
{
  return D(client, distribution) / 10; // 1 unidad de tiempo por cada 10 km
}

// Verificar si la entrega es posible dentro del tiempo permitido
bool IsDeliveryPossible(Object client, Object distribution)
{
  return T(client, distribution) <= MAX_TIME;
}

// Verificar si para todos los clientes existe al menos un centro de distribución donde se cumple el predicado
bool ForAll(Predicate p, Object clients[], int clientCount, Object distributions[], int distCount)
{
  for (int i = 0; i < clientCount; i++)
  {
    bool exists = false;
    for (int j = 0; j < distCount; j++)
    {
      if (p.evaluate(clients[i], distributions[j]))
      {
        exists = true;
        break;
      }
    }
    if (!exists)
    {
      return false;
    }
  }
  return true;
}
