#include "header.h"
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

int main(void) {
  bool_t go = TRUE;
  char arr[WHOLESIZE];
  GreetingMessage();
  do {
    fgets(arr, WHOLESIZE, stdin);
    go = Run(arr);
  } while (go);

  //Проверка на утечку памяти
  _CrtDumpMemoryLeaks();
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
  return 0;
}
