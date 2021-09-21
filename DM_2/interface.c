#include "Header.h"

bool_t Run(char* str) {

  bool_t err = NONE;
  int i = 0, len=0;
  char *cursor = str;

  uint m=0, n=0; //��������� ������������� �������
  uint ans, temp;

  while (str[len] != '\n' && len<SIZE)
    len++;

  //�������� �������. ������ ���� ���� �����

  if (len == 1 && str[0] == 'H')
    Help();

  else if (len == 1 && str[0] == 'Q') {
    printf("���������� ������...\n");
    return FALSE;
  }

  //���������� �������������� �����
  else {

    //���������� �������?
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

    //�������� ������� �����
    if (!NUM(*cursor)) {
      err = SYNTAX_ERROR;
      ErrorMessage(err);
      return TRUE;
    }

    while (NUM(*cursor)) {

      //������� ����� m
      temp = (uint)(*cursor) - (uint)'0';

      //���� ��� ��������� ����. ��������, �� ������
      if ((UINT_MAX - temp) / 10.0 < m) {
        err = TOO_LARGE_NUM;
        ErrorMessage(err);
        return TRUE;
      }

      m = m * 10 + temp;
      cursor++;
    }

    //����������� ����� m. 3 ������:
    // 1)��������� �������. ������ ����� n
    // 2)��������� ����������� ������ � ����� ������. ������� ������ ���������
    // 3)�����, ������


    //1)
    if ((*cursor) == ',') {

      cursor++;
      if (!NUM(*cursor)) {
        err = SYNTAX_ERROR;
        ErrorMessage(err);
        return TRUE;
      }

      while (NUM(*cursor)) {
        //������� ����� n
        temp = (uint)(*cursor) - (uint)'0';

        //���� ��� ��������� ����. ��������, �� ������
        if ((UINT_MAX - temp) / 10.0 < n) {
          err = TOO_LARGE_NUM;
          ErrorMessage(err);
          return TRUE;
        }
        n = n * 10 + temp;
        cursor++;
      }

      //���������� ����. 2 ������:
      //2.1) ��������� ����������� ������. ����� ���������� ����������
      //2.2) ����� ������

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
  printf("������ ���������� ������:\n");
  printf("H - ��������� �������\n");
  printf("Q - ���������� ������\n");
  printf("U(m,n) - ����� ����������\n");
  printf("A(m,n) - ����� ���������� ��� ����������\n");
  printf("P(n) - ����� ������������\n");
  printf("C(m,n) - ����� ���������\n");
  printf("S(m,n) - ����� ��������� ������� ����\n");
  printf("B(n) - ����� �����\n");

  printf("����� ������������ ������ (� �������������� ������) ���������� ");
  printf("�������� �������������� ����� �� ���� ��������� �������, ����� ");
  printf("��������� � ���� �������� ����������� 32 - ������� ������ �������(0..4294967295)\n");
}

void ErrorMessage(error_t err) {
  printf("ERROR: ");
  if (err == TOO_LARGE_NUM)
    printf("�������� ��� ��������� ������� �� ���������� � ���� 32-������� ������������ �����\n");
  else if (err == NOT_ENOUGH_MEMORY)
    printf("������������ ������ ��� �������� ����� (��������, ����� ������� �������)\n");
  else if (err == UNKNOWN_FUNCTION)
    printf("����������� ������������� �������\n");
  else if (err == SYNTAX_ERROR)
    printf("�������������� ������\n");
  else if (err == PARAMETRS)
    printf("�������� ����� ���������� � �������\n");
}

void GreetingMessage() {
  printf("������������ ������ �2. ������ ���������� ������������� �����\n");
  printf("������� H ��� �������\n");
}