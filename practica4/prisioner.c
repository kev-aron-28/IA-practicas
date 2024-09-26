#include <stdio.h>

#define COOPERATE 1
#define BETRAY 0

typedef struct {
  int playerA;
  int playerB;
} Payoffs;

int main(int argc, char const *argv[])
{
  Payoffs payoffsForEachPlayer[] = {
    { -3, -3 },
    { 0, -10 },
    { -10, 0 },
    { -5, -5 }
  };

  char * possibleChoices[] = {
    "Traicionar",
    "Cooperar"
  };

  int choiceA = COOPERATE;

  int choiceB = COOPERATE;

  Payoffs result;

  if(choiceA == COOPERATE && choiceB == COOPERATE) {
    result = payoffsForEachPlayer[0];
  } else if(choiceA == BETRAY && choiceA == COOPERATE) {
    result = payoffsForEachPlayer[1];
  } else if(choiceA == COOPERATE && choiceB == BETRAY) {
    result = payoffsForEachPlayer[2];
  } else if(choiceA == BETRAY && choiceB == BETRAY) {
    result = payoffsForEachPlayer[3];
  }

  printf("PRISIONERO A ELIGE %s\n", possibleChoices[choiceA]);

  printf("PRISIONERO B ELIGE %s\n", possibleChoices[choiceB]);

  printf("RESULTADO. CARCEL PARA A: %d Años | CARCEL PARA B: %d Años \n", result.playerA, result.playerB);
  return 0;
}
