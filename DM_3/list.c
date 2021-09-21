#include "Header.h"

void BuildPath(map_t* map) {
  int x = map->end.x, y = map->end.y;

  do {

    //left neighbour
    if (x > 0 && PATH(y, x - 1) && map->isVisited[y][x - 1] == true) {
      map->isVisited[y][x] = false;
      map->isPath[y][x] = true;
      x--;
    }

    //top neighbour
    else if (y > 0 && PATH(y - 1, x) && map->isVisited[y - 1][x] == true) {
      map->isVisited[y][x] = false;
      map->isPath[y][x] = true;
      y--;
    }

    //right neighbour
    else if (x < (map->length - 1) && PATH(y, x + 1) && map->isVisited[y][x + 1] == true) {
      map->isVisited[y][x] = false;
      map->isPath[y][x] = true;
      x++;
    }

    //bottom neighbour
    else if (y < (map->width - 1) && PATH(y + 1, x) && map->isVisited[y + 1][x] == true) {
      map->isVisited[y][x] = false;
      map->isPath[y][x] = true;
      y++;
    }

  } while (x != map->start.x || y != map->start.y);
}
