#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#pragma once

#include <stdio.h>
#include <stdlib.h>
#pragma warning (disable:4996)
#include <crtdbg.h>
#include <stdbool.h>
#include <Windows.h>
#define _CRTDBG_MAP_ALLOC

//for printing maze in different colours
enum {
  BLACK=0,
  WHITE=15,
  GREEN=2,
  RED=4
};

#define PATH(y,x) (map->map[y][x]=='O' || map->map[y][x]=='E' || map->map[y][x]=='S')

typedef enum {
  NONE = 0,
  CANNOT_READ_MAZE,
  NO_PATH_TO_EXIT,
  NOT_ENOUGH_MEMORY,
  ZERO_STARTING_POS,
  TWO_STARTING_POS,
  ZERO_EXIT_POS,
  INVALID_SYMBOL,
  NOT_CORRECT_SHAPE_OF_MAZE
}error_t;

typedef struct {
  int x, y;
} point_t; //was point visited or not

typedef struct {
  char** map; //matrix of points
  bool** isVisited; //matrix which shows if point was visited or not
  bool** isPath; //matrix which shows if point is in the way to exit
  int length, width;
  point_t start, end;
}map_t;

map_t* ReadMap(const char* filename);
void PrintMap(map_t* map, HANDLE hConsole);
void DFS(map_t* map);
void DestroyMap(map_t* map);

//stack
typedef struct elem_t {
  point_t point;
  struct elem_t* next, * prev;
}elem_t;

typedef struct {
  elem_t* last;
  elem_t* first;
}stack_t;
 
extern stack_t stack;

void StackPush(point_t p);
bool StackIsEmpty();
point_t StackTop();
void StackPop();
void DestroyStack();

//for image of graph
bool** AdjacencyMatrix(map_t* map);
void PrintMatrix(bool** matrix, map_t* map);
void DestroyMatrix(bool** matrix, map_t* map);

#endif //HEADER_H_INCLUDED