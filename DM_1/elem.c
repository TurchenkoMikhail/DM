#include "header.h"

//���������� ��� ��������
elem_t* ElemCompare(elem_t* elem1, elem_t* elem2) {

  int i,len1=0, len2=0;
  while (elem1->str[len1] != '\0')
    len1++;
  while (elem1->str[len2] != '\0')
    len2++;

  for (i = 0; (i < SIZE && MAX(elem1->str[i], elem2->str[i]) != '\0'); i++) {

    if (elem1->str[i] < elem2->str[i])
      return elem1;
    else if (elem1->str[i] > elem2->str[i])
      return elem2;
  }

  if (i != SIZE && elem1->str[i] == '\0' && elem2->str[i] != '\0')
    return elem1;
  else if (i != SIZE && elem2->str[i] == '\0' && elem1->str[i] != '\0')
    return elem2;

  return NULL; //������ �����
}

//�������� ������� � ������������ ���������
bool_t ElemAdd(set_t* set, char* str, error_t* error) {
  if (!set) //��������� �� ����������
    return FALSE;

  elem_t* temp = set->elem, * temp2 = temp;
  int i = 0;

  elem_t* newelem = (elem_t*)malloc(sizeof(elem_t));
  if (!newelem)
    return FALSE;

  if (str[i] != '"') {
    *error = SYNTAX_ERROR;
    free(newelem);
    return FALSE;
  }

  for (i = 1; i <= SIZE; ++i) {
    if (str[i] < ' ' || str[i]>'~') {//������������ ������
      *error = INCORRECT_SYMBOL;
      free(newelem);
      return FALSE;
    }
    //������ ���� ������������� ������ �������
    else if (str[i] == '"') {
      if (str[i+1] == '\n' || str[i+1] == ' ') { //����� ������
        newelem->str[i-1] = '\0';
        break;
      }
      else {
        *error = INCORRECT_SYMBOL;
        free(newelem);
        return FALSE;
      }
    }
    else if (i > SIZE) {
      *error = MEMORY;
      free(newelem);
      return FALSE;
    }
    else
      newelem->str[i-1] = str[i];
  }

  if (i > SIZE) {
    *error = MEMORY;
    free(newelem);
    return FALSE;
  }

  newelem->next = NULL;
  //��������� ������
  if (set->elem == NULL)
    set->elem = newelem;

  //��������� ��������
  else {

    //������� ���� �������� �������

    //�������� � ������?
    if (ElemCompare(temp, newelem) == newelem) {
      newelem->next = temp;
      set->elem = newelem;
      return TRUE;
    }
    //��������� � ��������/�����
    else if (ElemCompare(temp, newelem) == temp) {

      while (temp && (ElemCompare(temp, newelem) == temp)) {
        temp2 = temp;
        temp = temp->next;
      }

      //����� ����� �� �����, ������ ��� ����� ������� � ����� �� ���������
      if (temp && (ElemCompare(temp, newelem) == NULL)) {
        *error = ELEM_EXISTS;
        free(newelem);
        return FALSE;
      }

      newelem->next = temp;
      temp2->next = newelem;
    }

    else { //�������� ���������� -> ������ ��������� ����� �������
      *error = ELEM_EXISTS;
      free(newelem);
    }
  }
  return TRUE;
}

//������� ������� �� ���������
bool_t ElemRemove(set_t* set, elem_t* elem) {
  if (!elem || !set)
    return FALSE;

  elem_t* temp = set->elem, * temp2 = NULL;

  //������� ������ �������
  if (temp == elem) {
    temp2 = set->elem->next;
    free(temp);
    set->elem = temp2;
    return TRUE;
  }

  //������� �� ������ �������
  else {
    while (temp->next != elem)
      temp = temp->next;

    temp2 = temp->next->next;
    free(temp->next);
    temp->next = temp2;
    return TRUE;
  }
}

//��������, ����������� �� ������� ���������
//���� ��, ������ ��������� �� �������. ����� NULL
elem_t* ElemFind(set_t* set, char* str) {
  if (!set)
    return NULL;

  elem_t* temp = set->elem;
  int i = 0;
  while (temp) {

    for (i = 0; i < SIZE; i++) {
      if ((temp->str[i] != str[i+1])) {
       
        if (temp->str[i] == '\0' && str[i+1]=='"')
          i = (SIZE - 1);
        
        break;
      }

      if (str[i+1] == '\n' || str[i+1]=='"') {
        i = (SIZE-1);
        break;
      }
    }
    if (i == (SIZE-1))
      return temp;

    temp = temp->next;
  }
  return NULL;
}

//��������, ����� �� ��� ��������
bool_t IsElemsEqual(elem_t* elem1, elem_t* elem2) {
  if (!elem1 || !elem2)
    return FALSE;

  int i = 0;
  for (i = 0; i < SIZE; i++) {
    if (elem1->str[i] != elem2->str[i])
      return FALSE;
    if (elem1->str[i] == '\n')
      i = SIZE;
  }
  return TRUE;
}

//������� ����� ��������
elem_t* ElemCreateEqual(elem_t* elem) {
  int i = 0;
  elem_t* newelem = (elem_t*)malloc(sizeof(elem_t));
  if (!newelem)
    return NULL;

  for (i = 0; i < SIZE; i++) {
    newelem->str[i] = elem->str[i];
    if (elem->str[i] == '\n')
      i = SIZE;
  }
  newelem->next = NULL;
  return newelem;
}