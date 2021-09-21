//file to create adjacency matrix for graph and build image this graph in the internet
//website: https://graphonline.ru/create_graph_by_matrix

#include "Header.h"

bool** AdjacencyMatrix(map_t* map) {

  int x, y;
  int size=0;
  //calculate amount of vertexes
  typedef struct {
    int y, x;
    int numOfVertex;
  }point;

  for (y = 0; y < map->width; y++) {
    for (x = 0; x < map->length; x++) {
      if (PATH(y, x))
        size++;
    }
  }

  point* vertexes = (point*)malloc(size * sizeof(point));

  int num = 0;
  for (y = 0; y < map->width; y++) {
    for (x = 0; x < map->length; x++) {
      if (PATH(y, x)) {
        vertexes[num].x = x;
        vertexes[num].y = y;
        vertexes[num].numOfVertex = num;
        num++;
      }
    }
  }


  bool** matrix = (bool**)malloc(size * sizeof(bool*));
  if (!matrix)
    return NULL;

  int k,l;
  for (x = 0; x < size; x++)
    matrix[x] = (bool*)malloc(size * sizeof(bool));

  for (y = 0; y < size; y++) {
    for (x = 0; x < size; x++)
      matrix[y][x] = 0;
  }


  for (y = 0; y < map->width; y++){
    for (x = 0; x < map->length; x++) {

      if (PATH(y, x)) {

        //find this vertex in arr
        for (k = 0; k < size; k++) {
          if (vertexes[k].x == x && vertexes[k].y == y) {

            //left neihbour
            if (x > 0 && PATH(y, x - 1)) {
              for (l = 0; l < size; l++) {
                if (vertexes[l].x == x - 1 && vertexes[l].y == y)
                  matrix[vertexes[l].numOfVertex][vertexes[k].numOfVertex] = 1;
              }
            }

            //top neighbour
            if (y > 0 && PATH(y - 1, x)) {
              for (l = 0; l < size; l++) {
                if (vertexes[l].x == x && vertexes[l].y == y-1)
                  matrix[vertexes[l].numOfVertex][vertexes[k].numOfVertex] = 1;
              }
            }

            //right neighbour
            if (x < (map->length - 1) && PATH(y, x + 1)) {
              for (l = 0; l < size; l++) {
                if (vertexes[l].x == x + 1 && vertexes[l].y == y)
                  matrix[vertexes[l].numOfVertex][vertexes[k].numOfVertex] = 1;
              }
            }

            //bottom neighbour
            if (y < (map->width - 1) && PATH(y + 1, x)) {
              for (l = 0; l < size; l++) {
                if (vertexes[l].x == x && vertexes[l].y == y+1)
                  matrix[vertexes[l].numOfVertex][vertexes[k].numOfVertex] = 1;
              }
            }

            break;
          }
        }
      }
    }
  }
  free(vertexes);
  return matrix;
}

void PrintMatrix(bool** matrix, map_t* map) {
  int y, x, size=0;
  for (y = 0; y < map->width; y++) {
    for (x = 0; x < map->length; x++) {
      if (PATH(y, x))
        size++;
    }
  }

  for (y = 0; y < size; y++) {
    for (x = 0; x < size; x++)
      printf("%i ", matrix[y][x]);
    printf("\n");
  }
}

void DestroyMatrix(bool** matrix, map_t* map) {
  int y, x, size = 0;
  for (y = 0; y < map->width; y++) {
    for (x = 0; x < map->length; x++) {
      if (PATH(y, x))
        size++;
    }
  }

  for (y = 0; y < size; y++)
    free(matrix[y]);
  free(matrix);
}