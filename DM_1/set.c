#include "header.h"

list_of_sets list;
set_t* tail = NULL;
 
//����� ��������� �� �����
void SetPrint(set_t * set) {
  if (!set)
    return;

  elem_t* temp = set->elem;
  while (temp) {
    printf("%s\n", temp->str);
    temp = temp->next;
  }
}

//����� ���� �������� �� �����
void SetAllPrint() {
  set_t* temp = list.head;
  printf("All sets:\n");
  while (temp) {
    printf("\"%s\"", temp->name);
    temp = temp->next;
    if (temp)
      printf(", ");
  }
  printf("\n");
}

//�������� ��������� (������ � ���������)
set_t* SetCreate(char* name, error_t* err) {
  int i = 0, j=0;
  set_t* set = (set_t*)malloc(sizeof(set_t));
  if (!set) {
    *err = MEMORY;
    return NULL;
  }
  bool_t EXISTS = FALSE;
  set_t* temp = list.head;
  //��� ���������
  for (i = 0; i < NAMESIZE; i++) {

    if (name[i] != '"' && name[i] != '\n') {
      set->name[j] = name[i];
      j++;
    }

    if (name[i] == '\n') {
      set->name[j] = '\0';
      i = NAMESIZE; //����� �� �����
    }
  }
  set->next = NULL;
  set->elem = NULL;

  //��������, ��� ��������� � ����� ������ ������ ���
  temp = list.head;
  while (temp && EXISTS == FALSE) {
    EXISTS = TRUE;

    i = 0;
    while (temp->name[i] != '\0' || set->name[i] != '\0' || i <= NAMESIZE) {

      if (temp->name[i] != set->name[i]) {
        EXISTS = FALSE;
        break;
      }

      else if (temp->name[i] == '\0' && set->name[i] == '\0') {
        EXISTS = TRUE;
        break;
      }
      i++;
    }
    if (EXISTS)
      break;

    temp = temp->next;
  }

  if (EXISTS==TRUE) {
    *err = SET_EXISTS;
    free(set);
    return NULL;
  }

  //��������� ����� ��������� � ������ ��������
  if (list.head == NULL) {
    list.head = set;
    tail = list.head;
    tail->next = NULL;
  }
  else {
    tail->next = set;
    tail = tail->next;
    tail->next = NULL;
  }
  return set;
}

set_t* SetFind(char* name) {
  set_t* temp = list.head;
  bool_t wasFound = TRUE;
  int i;

  while (temp) {
    wasFound = TRUE;
    for (i = 1; i < SIZE; ++i) {

      if (name[i] == '\0' || name[i] == '"') {
        i = SIZE;
      }

      else if (temp->name[i-1] != name[i])
        wasFound = FALSE;
      

    }
    if (wasFound)
      return temp;
      
    temp = temp->next;
  }

  //����� �� �����, �� �� ����� ���������
  return NULL;
}

//�������� ������������� ��������
void SetDestroy(set_t* set) {
  if (!set)
    return;

  elem_t* temp = set->elem, *temp2 = set->elem;
  set_t* findset = list.head;

  //������� ������
  if (findset == set) 
    list.head = findset->next;

  //������� ������, ������ � �.�...
  else {
    while (findset->next != set)
      findset = findset->next;

    //���� ������� ��������� ���������, �� ��������� tail �� �����
    if (set->next == NULL)
      tail = findset;

    //�� ������ �����
    findset->next = findset->next->next;
  }
  //������� ���������
  while (temp2) {
    temp2 = temp2->next;
    free(temp);
    temp = temp2;
  }
  if(temp)
    free(temp);
  free(set);
}

//������� �������� ���������
int SetPower(set_t* set) {
  int power = 0;
  elem_t* temp = set->elem;
  while (temp) {
    temp = temp->next;
    power++;
  }
  return power;
}

//����������� ��������� (��������������� �������)
set_t* SetCopy(set_t* set) {
  if (!set)
    return NULL;

  elem_t* temp=set->elem, *temp2;
  set_t* newset = (set_t*)malloc(sizeof(set_t));
  if (!newset)
    return NULL;

  newset->elem = ElemCreateEqual(temp);
  temp2 = newset->elem;
  temp = temp->next;

  while (temp) {
    temp2->next = ElemCreateEqual(temp);

    if (!temp2->next) {
      SetDestroy(newset);
      return NULL;
    }

    temp = temp->next;
    temp2 = temp2->next;
  }

  return newset;
}

//����������� ��������
set_t* SetUnion(set_t* set1, set_t* set2) {
  set_t* set3 = (set_t*)malloc(sizeof(set_t));
  if (!set3)
    return NULL;

  set3->elem = NULL;
  set3->next = NULL;

  elem_t* temp1 = NULL, * temp2 = NULL, * newelem = NULL, * temp = set3->elem;
  elem_t* search = NULL;

  //����� ��������� �� ���������� � ����������
  if (!set1 || !set2) {
    free(set3);
    return NULL;
  }

  //������ ��������� ������
  else if (!set1->elem) {

    //������ ��������� ��� ����������, ��� ������
    if (!set2->elem) {
      SetAddToList(set3);
      return set3;//������� ������ ���������
    }
    //������� ������ ���������, ���� ��� �� ������ 
    else {
      free(set3);
      set3 = SetCopy(set2);
      SetAddToList(set3);
      return set3;
    }
  }

  //������ ��������� ������
  else if (!set2->elem) {

    //���� ������ ���� ������, �� ����
    //������ ���������� ����
    free(set3);
    set3 = SetCopy(set1);
    SetAddToList(set3);
    return set3;
  }

  //��� ��������� ���������� � ��������
  temp1 = set1->elem;
  temp2 = set2->elem;
  newelem = NULL;
  temp = set3->elem;
  search = set3->elem;

  while (temp1 != NULL && temp2 != NULL) {

    newelem = ElemCompare(temp1, temp2);

    if (newelem == temp1) {
      newelem = ElemCreateEqual(temp1);
      temp1 = temp1->next;
    }

    else if (newelem == temp2) {
      newelem = ElemCreateEqual(temp2);
      temp2 = temp2->next;
    }

    else { //�������� �����, ��������� ������ ����
      newelem = ElemCreateEqual(temp1);
      temp1 = temp1->next;
      temp2 = temp2->next;
    }

    //��������� newelem �� ��������� � ���������, ��� ������ �������� ���
    search = set3->elem;
    while (search) {
      if (IsElemsEqual(search, newelem)) {
        free(newelem);
        newelem = NULL;
        break;
      }
      search = search->next;
    }
    //���� ������ �������� �� ��������� ���, ������� ���
    if (newelem) {
      if (temp == NULL) {
        set3->elem = newelem;
        temp = newelem;
      }
      else {
        temp->next = newelem;
        temp = temp->next;
      }
    }
  }

  //���������� ����� �� ������ �� �������� � ���������
  if (temp1 == NULL)
    temp1 = temp2;

  while (temp1) {
    newelem = ElemCreateEqual(temp1);

    //��������
    search = set3->elem;
    while (search) {
      if (IsElemsEqual(search, newelem)) {
        free(newelem);
        newelem = NULL;
        break;
      }
      search = search->next;
    }

    temp1 = temp1->next;
    if (newelem) {
      temp->next = newelem;
      temp = temp->next;
    }
  }

  //������� ��������� � ������ ��������
  SetAddToList(set3);
  return set3;
}

//����������� ��������
set_t* SetIntersection(set_t* set1, set_t* set2) {
  set_t* set3 = (set_t*)malloc(sizeof(set_t));
  if (!set3)
    return NULL;

  set3->elem = NULL;
  set3->next = NULL;

  //���� �����-�� ��������� �� ����������
  if (!set1 || !set2) {
    free(set3);
    return NULL;
  }

  //���� �����-���� ��������� ������
  else if (!set1->elem || !set2->elem) {
    SetAddToList(set3);
    return set3;
  }
  
  elem_t* temp1 = set1->elem, * temp2 = set2->elem, * newelem = NULL, * temp = set3->elem;
  elem_t* search = set2->elem;

  while (temp1 != NULL && temp2 != NULL) {

    search = set2->elem;
    while (search) {
      if (IsElemsEqual(search, temp1)) {
        newelem = ElemCreateEqual(temp1);
        if (temp == NULL) {
          set3->elem = newelem;
          temp = newelem;
        }
        else {
          temp->next = newelem;
          temp = temp->next;
        }
      }
      search = search->next;
    }
    temp1 = temp1->next;
  }
  SetAddToList(set3);
  return set3;
}

//�������� ��������
set_t* SetDifference(set_t* set1, set_t* set2) {
  set_t* set3 = (set_t*)malloc(sizeof(set_t));
  if (!set3)
    return NULL;

  set3->elem = NULL;
  set3->next = NULL;

  if (!set1 || !set2) {
    free(set3);
    return NULL;
  }
  //������ ��������� ������
  else if (!set1->elem) {
    SetAddToList(set3);
    return set3;
  }

  //������ ��������� ������
  else if (!set2->elem) {
    free(set3);
    set3 = SetCopy(set1);
    SetAddToList(set3);
    return set3;
  }

  elem_t* temp1 = set1->elem, * temp2 = set2->elem, * newelem = NULL, * temp = set3->elem;
  elem_t* search = set2->elem;

  while (temp1 != NULL && temp2 != NULL) {
    newelem = ElemCreateEqual(temp1);

    search = set2->elem;
    while (search) {
      if (IsElemsEqual(search, temp1)) {
        free(newelem);
        newelem = NULL;
        break;
      }
      search = search->next;
    }
    if (newelem) {
      if (temp == NULL) {
        set3->elem = newelem;
        temp = newelem;
      }
      else {
        temp->next = newelem;
        temp = temp->next;
      }
    }
    temp1 = temp1->next;
  }
  SetAddToList(set3);
  return set3;
}

//�������������� �������� ��������
set_t* SetSymmetricDifference(set_t* set1, set_t* set2) {
  set_t* set3 = (set_t*)malloc(sizeof(set_t));
  if (!set3)
    return NULL;

  set3->elem = NULL;
  set3->next = NULL;

  if (!set1 || !set2) {
    free(set3);
    return NULL;
  }

  //������ ��������� ������
  if (!set1->elem) {
    //������ ��������� ������
    if (!set2->elem) {
      SetAddToList(set3);
      return set3;
    }
    
    //������ ��������
    free(set3);
    set3 = SetCopy(set2);
    SetAddToList(set3);
    return set3;
  }

  //������ ������
  else if (!set2->elem) {
    free(set3);
    set3 = SetCopy(set1);
    SetAddToList(set3);
    return set3;
  }

  elem_t* temp1 = set1->elem, * temp2 = set2->elem, * newelem = NULL, * temp = set3->elem;
  elem_t* search1 = set3->elem, *search2 = NULL;

  while (temp1 != NULL && temp2 != NULL) {

    newelem = ElemCompare(temp1, temp2);

    if (newelem == temp1) {
      newelem = ElemCreateEqual(temp1);
      temp1 = temp1->next;
      search2 = set2->elem;
    }

    else if (newelem == temp2) {
      newelem = ElemCreateEqual(temp2);
      temp2 = temp2->next;
      search2 = set1->elem;
    }

    else { //�������� �����, �� ��������� ��
      temp1 = temp1->next;
      temp2 = temp2->next;
      free(newelem);
      newelem = NULL;
    }

    //��������� newelem �� ��������� � ���������, ��� ������ �������� ��� �� ������ � ������� ���������
    search1 = set3->elem;
    while (newelem&&(search1||search2)) {
      if (IsElemsEqual(search1, newelem)||IsElemsEqual(search2, newelem)) {
        free(newelem);
        newelem = NULL;
        break;
      }
      if(search1)
        search1 = search1->next;
      if (search2)
        search2 = search2->next;
    }
    //���� ������ �������� �� ���������� ���, ������� ���
    if (newelem) {
      if (temp == NULL) {
        set3->elem = newelem;
        temp = newelem;
      }
      else {
        temp->next = newelem;
        temp = temp->next;
      }
    }
  }

  //���������� ����� �� ������ �� �������� � ���������
  if (temp1 == NULL) {
    temp1 = temp2;
    search2 = set1->elem;
  }
  else
    search2 = set2->elem;
  

  while (temp1) {
    newelem = ElemCreateEqual(temp1);

    //��������
    search1 = set3->elem;
    while (search1||search2) {
      if (IsElemsEqual(search1, newelem)|| IsElemsEqual(search2, newelem)) {
        free(newelem);
        newelem = NULL;
        break;
      }
      if(search1)
        search1 = search1->next;
      if(search2)
        search2 = search2->next;
    }

    temp1 = temp1->next;
    if (newelem) {
      temp->next = newelem;
      temp = temp->next;
    }
  }
  SetAddToList(set3);
  return set3;
}

//�������� ��������� set1 � set2
bool_t SetCheckContain(set_t* set1, set_t* set2) {
  if (!set1 || !set2)
    return FALSE;

  elem_t* temp = set1->elem;
  elem_t* search = set2->elem;

  bool_t ELEM_EXISTS = FALSE;

  while (temp) {
    search = set2->elem;
    ELEM_EXISTS = FALSE;

    while (search) {
      if (IsElemsEqual(search, temp))
        ELEM_EXISTS = TRUE;

      search = search->next;
    }

    if (!ELEM_EXISTS)
      return FALSE;

    temp = temp->next;
  }
  return TRUE;
}

//����� ��������� ��� ������� arg � ������� �� ����������
set_t* SetFindInStr(char* str, int arg, error_t* err) {
  char* cursor = str;
  int i = 0, comma = 0; //����� ������ ������� 
  set_t* set = NULL;
  while (i < WHOLESIZE) {

    if (*cursor == '"') {
      comma++;
      if (comma == arg) {
        set = SetFind(cursor);
        break;
      }
    }
    cursor++;
    i++;
  }

  if (!set) 
    *err = SET_DOES_NOT_EXIST;
  
  if (i == WHOLESIZE) 
    *err = MEMORY;
  
  if (*err)
    return NULL;

  return set;
}

//���������� ��� ������ ��������� (��� ������ �����������, ����������� � �.�.)
char* SetNewName(char* str) {
  char* name = (char*)malloc(NAMESIZE * sizeof(char));
  if (!name)
    return NULL;
  int j = 0;
  char* cursor = str;
  int i = 0, comma = 0; //����� ����� ������� 
  while (i < SIZE) {

    if (*cursor == '"') {
      comma++;
      if (comma == 5)
        break;
      
    }
    cursor++;
    i++;
  }
  //������
  if (i == SIZE) {
    free(name);
    return NULL;
  }
  j = i;
  for (i; i < (j+NAMESIZE); i++) {
    name[i-j] = str[i+1];
    if (i != j && name[i - j] == '"') {
      name[i - j] = '\0';
      break;
    }
    
  }
  return name;
}

//���������� ��������� � ������ ��������
void SetAddToList(set_t* set) {
  if (tail == NULL) {
    list.head = set;
    tail = set;
  }
  else {
    tail->next = set;
    tail = tail->next;
  }
}

