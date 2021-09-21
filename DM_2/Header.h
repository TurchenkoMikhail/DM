#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <limits.h>

#define SIZE 4096

#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define NUM(x) ((x)>='0'&&(x)<='9')

typedef enum {
  FALSE = 0,
  TRUE = 1
}bool_t;

typedef enum {
  NONE = 0,
  TOO_LARGE_NUM = 1,
  NOT_ENOUGH_MEMORY=2,
  UNKNOWN_FUNCTION = 3,
  SYNTAX_ERROR = 4,
  PARAMETRS = 5,
}error_t;

typedef unsigned int uint;

//������������� �������
uint U(uint m, uint n, error_t* err); //����������
uint A(uint m, uint n, error_t* err); //���������� ��� ����������
uint P(uint n, error_t* err);         //������������
uint C(uint m, uint n, error_t* err); //���������
uint S(uint m, uint n, error_t* err); //����� ��������� 2-��� ����
uint B(uint m, error_t* err);         //����� �����

//���������
bool_t Run(char* str);
void ErrorMessage(error_t err);
uint CalculateFuncOneParametr(char* str, uint n, error_t* err);
uint CalculateFuncTwoParametrs(char* str, uint m, uint n, error_t* err);
void Help();
void GreetingMessage();
#endif //HEADER_H_INCLUDED