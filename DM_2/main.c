#include "Header.h"

int main(void) {
  setlocale(LC_ALL, "Rus");

  bool_t go = TRUE;
  char arr[SIZE];
  GreetingMessage();

  do {
    fgets(arr, SIZE, stdin);
    go = Run(arr);
  } while (go);

  return 0;
}