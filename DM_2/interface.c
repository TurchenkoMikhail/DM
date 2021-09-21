#include "Header.h"

bool_t Run(char* str) {

  bool_t err = NONE;
  int i = 0, len=0;
  char *cursor = str;

  uint m=0, n=0; //аргументы комбинаторных функций
  uint ans, temp;

  while (str[len] != '\n' && len<SIZE)
    len++;

  //Проверка команды. Должна быть одна буква

  if (len == 1 && str[0] == 'H')
    Help();

  else if (len == 1 && str[0] == 'Q') {
    printf("Завершение работы...\n");
    return FALSE;
  }

  //Вычисление комбинаторного числа
  else {

    //Допустимая функция?
    if ((*cursor) != 'U' && (*cursor) != 'A' && (*cursor) != 'P' && (*cursor) != 'C' \
      && (*cursor) != 'S' && (*cursor) != 'B') {

      if ((*cursor) >= '0' && (*cursor) <= '9')
        return TRUE;

      err = UNKNOWN_FUNCTION;
      ErrorMessage(err);
      return TRUE;
    }
    cursor++;

    if (*cursor != '(') {
      err = SYNTAX_ERROR;
      ErrorMessage(err);
      return TRUE;
    }
    cursor++; 

    //начинаем подсчет чисел
    if (!NUM(*cursor)) {
      err = SYNTAX_ERROR;
      ErrorMessage(err);
      return TRUE;
    }

    while (NUM(*cursor)) {

      //подсчет числа m
      temp = (uint)(*cursor) - (uint)'0';

      //Если оно превышает макс. значение, то ошибка
      if ((UINT_MAX - temp) / 10.0 < m) {
        err = TOO_LARGE_NUM;
        ErrorMessage(err);
        return TRUE;
      }

      m = m * 10 + temp;
      cursor++;
    }

    //закончилось число m. 3 случая:
    // 1)Встретили запятую. Читаем число n
    // 2)Встретили закрывающую скобку и конец строки. Функция одного аргумента
    // 3)Иначе, ошибка


    //1)
    if ((*cursor) == ',') {

      cursor++;
      if (!NUM(*cursor)) {
        err = SYNTAX_ERROR;
        ErrorMessage(err);
        return TRUE;
      }

      while (NUM(*cursor)) {
        //подсчет числа n
        temp = (uint)(*cursor) - (uint)'0';

        //Если оно превышает макс. значение, то ошибка
        if ((UINT_MAX - temp) / 10.0 < n) {
          err = TOO_LARGE_NUM;
          ErrorMessage(err);
          return TRUE;
        }
        n = n * 10 + temp;
        cursor++;
      }

      //Закончился цикл. 2 случая:
      //2.1) Встретили закрывающую скобку. Конец считывания параметров
      //2.2) Иначе ошибка

      if ((*cursor) == ')' && (*(cursor + 1) == '\n')) {

        ans = CalculateFuncTwoParametrs(str, m, n, &err);

        if (!err)
          printf("%c(%u,%u) == %u\n",str[0], m, n, ans);
        else
          ErrorMessage(err);
      }
      else {
        err = SYNTAX_ERROR;
        ErrorMessage(err);
        return TRUE;
      }
      
    }

    //2)
    else if ((*cursor) == ')' && (*(cursor+1)=='\n')) {

      n = m;
      ans = CalculateFuncOneParametr(str, n, &err);

      if (!err) 
        printf("%c(%u) == %u\n",str[0], n, ans);
      
      else
        ErrorMessage(err);
    }

    //3)
    else {
      err = SYNTAX_ERROR;
      ErrorMessage(err);
      return TRUE;
    }
  }

  return TRUE;
}

uint CalculateFuncOneParametr(char* str, uint n, error_t* err) {

  uint ans=0;
  if (str[0] == 'P')
    ans = P(n, err);
  else if (str[0] == 'B')
    ans = B(n, err);
  else
    *err = PARAMETRS;

  return ans;
}

uint CalculateFuncTwoParametrs(char* str,uint m, uint n, error_t* err) {
  uint ans=0;

  if (str[0] == 'U')
    ans = U(m, n, err);
  else if (str[0] == 'A')
    ans = A(m, n, err);
  else if (str[0] == 'C')
    ans = C(m, n, err);
  else if (str[0] == 'S')
    ans = S(m, n, err);
  else
    *err = PARAMETRS;

  return ans;
}

void Help() {
  printf("Список допустимых команд:\n");
  printf("H - получение справки\n");
  printf("Q - завершение работы\n");
  printf("U(m,n) - число размещений\n");
  printf("A(m,n) - число размещений без повторений\n");
  printf("P(n) - число перестановок\n");
  printf("C(m,n) - число сочетаний\n");
  printf("S(m,n) - число Стирлинга второго рода\n");
  printf("B(n) - число Белла\n");

  printf("Пакет обеспечивает точное (в математическом смысле) вычисление ");
  printf("значения комбинаторного числа во всех возможных случаях, когда ");
  printf("параметры и само значение представимы 32 - битными целыми числами(0..4294967295)\n");
}

void ErrorMessage(error_t err) {
  printf("ERROR: ");
  if (err == TOO_LARGE_NUM)
    printf("аргумент или результат функции не представим в виде 32-битного беззнакового числа\n");
  else if (err == NOT_ENOUGH_MEMORY)
    printf("недостаточно памяти для подсчета числа (вероятно, число слишком большое)\n");
  else if (err == UNKNOWN_FUNCTION)
    printf("неизвестная комбинаторная функция\n");
  else if (err == SYNTAX_ERROR)
    printf("синтаксическая ошибка\n");
  else if (err == PARAMETRS)
    printf("неверное число параметров у функции\n");
}

void GreetingMessage() {
  printf("Лабораторная работа №2. Точные вычисления комбинаторных чисел\n");
  printf("Введите H для справки\n");
}