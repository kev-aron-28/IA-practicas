#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_TIME 48

typedef struct object
{
  char name[50];
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

int main(int argc, char const *argv[])
{
  Object clients[2] = {{"Client1"}, {"Client2"}};
  Object distribution[2] = {{"CD1"}, {"CD2"}};

  Predicate delivery_predicate = {"DeliveryPossible", IsDeliveryPossible};

  // Check if delivery is possible for all clients from at least one distribution center
  if (ForAll(delivery_predicate, clients, 2, distribution, 2))
  {
    printf("All clients can be served by at least one distribution center.\n");
  }
  else
  {
    printf("Not all clients can be served within the time limit.\n");
  }

  return 0;
}

// Calculate the distance between a client and a distribution center
int D(Object client, Object distribution)
{
  if (strcmp(client.name, "Client1") == 0 && strcmp(distribution.name, "CD1") == 0)
  {
    return 40;
  }
  else if (strcmp(client.name, "Client1") == 0 && strcmp(distribution.name, "CD2") == 0)
  {
    return 60;
  }
  else if (strcmp(client.name, "Client2") == 0 && strcmp(distribution.name, "CD1") == 0)
  {
    return 35;
  }
  else if (strcmp(client.name, "Client2") == 0 && strcmp(distribution.name, "CD2") == 0)
  {
    return 50;
  }

  return 100; // High value means far distance
}

// Calculate the delivery time based on the distance
int T(Object client, Object distribution)
{
  return D(client, distribution) / 10; // 1 time unit per 10 km
}

// Predicate to check if the delivery is possible within the allowed time
bool IsDeliveryPossible(Object client, Object distribution)
{
  return T(client, distribution) <= MAX_TIME;
}

// Check if for all clients there exists at least one distribution center where the predicate holds
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

        break; // Found one valid distribution for the client, so move to the next client
      }
    }
  
    if (!exists)
    {
      return false; // If no distribution is found for any client, return false
    }
  
  }

  return true; // If all clients are served by at least one distribution
}

// Check if there exists at least one combination where the predicate holds
bool ThereExists(Predicate p, Object clients[], int clientCount, Object distributions[], int distCount)
{
  for (int i = 0; i < clientCount; i++)
  {
    for (int j = 0; j < distCount; j++)
    {
      if (p.evaluate(clients[i], distributions[j]))
      {
  
        return true; // If any valid combination is found, return true
      }
    }
  }

  return false; // If no valid combination is found
}
