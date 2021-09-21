#include "Header.h"

error_t err = NONE;

void ErrorMessage() {
  if (!err)
    return;

  printf("ERROR: ");

  if (err == NO_PATH_TO_EXIT)
    printf("there is no path to exit in this maze");
  else if (err == NOT_ENOUGH_MEMORY)
    printf("not enough memory");
  else if (err == ZERO_STARTING_POS)
    printf("there is no starting position in this maze");
  else if (err == TWO_STARTING_POS)
    printf("there are >1 starting positions in this maze");
  else if (err == ZERO_EXIT_POS)
    printf("there is no exit from this maze");
  else if (err == INVALID_SYMBOL)
    printf("there are invalid symbols in the map on this maze");
  else if (err == CANNOT_READ_MAZE)
    printf("file with maze was not found");
  else if (err == NOT_CORRECT_SHAPE_OF_MAZE)
    printf("this maze does not have rectangular shape");

  printf("\n\n");
}

//if there was an error in allocating memory for map
void FreeMap(map_t* map, int lenmap, int lenIsVisited, int lenIsPath) {
  int j;
  for (j = 0; j < lenmap; j++)
    free(map->map[j]);
  free(map->map);

  if (map->isVisited) {
    for (j = 0; j < lenIsVisited; j++)
      free(map->isVisited[j]);
    free(map->isVisited);
  }

  if (map->isPath) {
    for (j = 0; j < lenIsPath; j++)
      free(map->isPath[j]);
    free(map->isPath);
  }

  free(map);
}

map_t* ReadMap(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    err = CANNOT_READ_MAZE;
    return NULL;
  }

  map_t* newmap = (map_t*)malloc(sizeof(map_t));
  if (!newmap)
    return NULL;

  newmap->isVisited = NULL;
  newmap->isPath = NULL;
  char c;
  int size, lineSize=0, numOfLines,numOfColumn;
  int i,j;
  bool startIsFound = false, endIsFound = false, endOfLineIsFound = false;

  fseek(file, 0, SEEK_END);
  size = ftell(file); //size of whole file
  fseek(file, 0, SEEK_SET);
  //reading size of one line
  do {
    c = fgetc(file);
    lineSize++;
  } while (c != '\n');
  lineSize++;

  numOfLines = size / lineSize;
  numOfColumn = (size-2*numOfLines) / numOfLines;

  newmap->width = numOfLines;
  newmap->length = numOfColumn;

  //allocating memory for map

  newmap->map = (char**)malloc(newmap->width * sizeof(char*));
  if (!newmap->map)
    return NULL;

  for (i = 0; i < newmap->width; i++) {
    newmap->map[i] = (char*)malloc(newmap->length * sizeof(char));

    if (!newmap->map[i]) {
      FreeMap(newmap, i, 0, 0); //free new map and previously allocated memory
      return NULL;
    }

  }

  newmap->isVisited = (bool**)malloc(newmap->width * sizeof(bool*));
  if (!newmap->isVisited) {
    FreeMap(newmap, newmap->width, 0, 0); 
    return NULL;
  }

  for (i = 0; i < newmap->width; i++) {
    newmap->isVisited[i] = (bool*)malloc(newmap->length * sizeof(bool));

    if (!newmap->isVisited[i]) {
      FreeMap(newmap, newmap->width, i, 0);
      return NULL;
    }

    for (j = 0; j < newmap->length; j++)
      newmap->isVisited[i][j] = false;
  }

  newmap->isPath = (bool**)malloc(newmap->width * sizeof(bool*));
  if (!newmap->isPath) {
    FreeMap(newmap, newmap->width, newmap->width, 0);
    return NULL;
  }

  for (i = 0; i < newmap->width; i++) {
    newmap->isPath[i] = (bool*)malloc(newmap->length * sizeof(bool));

    if (!newmap->isPath[i]) {
      FreeMap(newmap, newmap->width, newmap->width, i);
      return NULL;
    }

    for (j = 0; j < newmap->length; j++)
      newmap->isPath[i][j] = false;
  }



  fseek(file, 0, SEEK_SET);
  for (i = 0; i < newmap->width; i++) {

    endOfLineIsFound = false;
    for (j = 0; j < newmap->length; j++) {
      c = fgetc(file);
      newmap->map[i][j] = c;

      if (i != 0 && j == 0 && newmap->map[i][j] != '\n' && endOfLineIsFound==false) {
        err = NOT_CORRECT_SHAPE_OF_MAZE;
        FreeMap(newmap, newmap->width, newmap->width, newmap->width);
        return NULL;
      }

      //if invalid symbol in map
      if (newmap->map[i][j] != 'O' && newmap->map[i][j] != 'X' && newmap->map[i][j] != 'S'\
        && newmap->map[i][j] != 'E' && newmap->map[i][j] != '\n') {
        err = INVALID_SYMBOL;
        FreeMap(newmap, newmap->width, newmap->width, newmap->width);
        return NULL;
      }

      if (newmap->map[i][j] == '\n') { //do not read '\n' symbol
        endOfLineIsFound = true;
        j--;
      }
      //starting position
      else if (newmap->map[i][j] == 'S') {

        //2 starting positions were found
        if (startIsFound == true) {
          FreeMap(newmap, newmap->width, newmap->width, newmap->width);
          err = TWO_STARTING_POS;
          return NULL;
        }
        startIsFound = true;
        newmap->start.x = j;
        newmap->start.y = i;
      }

      else if (newmap->map[i][j] == 'E')
        endIsFound = true;
    }
  }

  if (startIsFound == false) {
    FreeMap(newmap, newmap->width, newmap->width, newmap->width);
    err = ZERO_STARTING_POS;
    return NULL;
  }

  else if (endIsFound == false) {
    FreeMap(newmap, newmap->width, newmap->width, newmap->width);
    err = ZERO_EXIT_POS;
    return NULL;
  }

  fclose(file);
  return newmap;
}

void PrintMap(map_t* map, HANDLE hConsole) {
  int i, j;
  for (i = 0; i < map->width; i++) {
    for (j = 0; j < map->length; j++) {
      if (PATH(i,j)) {
        //black bachground, red font
        SetConsoleTextAttribute(hConsole, (WORD)((BLACK << 4) | RED));
      }
      else {
        //black bachground, white font
        SetConsoleTextAttribute(hConsole, (WORD)((BLACK << 4) | WHITE));
      }
      printf("%c", map->map[i][j]);
    }
    printf("\n");
  }
  printf("\n\n");
}

//find path to exit
void DFS(map_t* map) {
  int x, y; //coordinates of current point
  point_t start = map->start, temp, np; //next point
  map->isVisited[start.y][start.x] = false;

  StackPush(start);

  while (!StackIsEmpty()) {

    temp = StackTop();
    StackPop();
    x = temp.x;
    y = temp.y;

    map->isPath[y][x] = true;

    //if exit
    if (map->map[y][x] == 'E') {
      map->isVisited[start.y][start.x] = true;
      map->isVisited[y][x] = true;
      map->end.y = y;
      map->end.x = x;

      DestroyStack();
      return;
    }

    if (map->isVisited[y][x] == false) {
      map->isVisited[y][x] = true;

      //if neighbours are not visited, push them into stack

      //left neihbour
      if (x > 0 && PATH(y, x-1) && map->isVisited[y][x - 1] == false) {
        np.x = x - 1;
        np.y = y;
        StackPush(np);
      }

      //top neighbour
      if (y > 0 && PATH(y-1, x) && map->isVisited[y-1][x] == false) {
        np.x = x;
        np.y = y-1;
        StackPush(np);
      }

      //right neighbour
      if (x < (map->length-1) && PATH(y, x + 1) && map->isVisited[y][x+1] == false) {
        np.x = x+1;
        np.y = y;
        StackPush(np);
      }

      //bottom neighbour
      if (y < (map->width-1) && PATH(y+1, x) && map->isVisited[y+1][x] == false) {
        np.x = x;
        np.y = y+1;
        StackPush(np);
      }
      
    }
  } //while !StackIsEmpty

  //stack is empty, therefore there is no path to exit
  err = NO_PATH_TO_EXIT;

}

void PrintPath(map_t* map, HANDLE hConsole) {
  int i, j;
  for (i = 0; i < map->width; i++) {
    for (j = 0; j < map->length; j++) {

      if (map->isPath[i][j]==true){
        //black bachground, green font (whole passed path, not only corrected)
        SetConsoleTextAttribute(hConsole, (WORD)((BLACK << 4) | GREEN));
      }
      else if (PATH(i,j)){
        //black bachground, red font (all paths) 
        SetConsoleTextAttribute(hConsole, (WORD)((BLACK << 4) | RED));
      }
      else {
        //black bachground, white font (wall)
        SetConsoleTextAttribute(hConsole, (WORD)((BLACK << 4) | WHITE));
      }

      printf("%c", map->map[i][j]);
    }
    printf("\n");
  }
  SetConsoleTextAttribute(hConsole, (WORD)((BLACK << 4) | WHITE));
}

void DestroyMap(map_t* map) {
  int i;
  for (i = 0; i < map->width; i++) {
    free(map->map[i]);
    free(map->isVisited[i]);
    free(map->isPath[i]);
  }
  free(map->isPath);
  free(map->isVisited);
  free(map->map);
  free(map);
}

int main(void) {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //change colour in stdout

  //array of mazes
  bool** matrix = NULL;
  map_t* map = NULL;
  int size = 5;
  char* mazes[] = {"verysmallmap.txt","smallmap.txt","midmap.txt","bigmap.txt","verybigmap.txt"};

  for (int i = 0; i < size; i++) {
    map = ReadMap(mazes[i]);
    if (err)
      ErrorMessage();
    else {

      //building adjacency matrix to create image of graph

      //matrix = AdjacencyMatrix(map);
      //PrintMatrix(matrix, map);
      //DestroyMatrix(matrix, map);

      printf("Map #%i: \"%s\", size:%ix%i\n", i + 1, mazes[i], map->length, map->width);
      PrintMap(map, hConsole);
      DFS(map);

      if (err) //error can be only if there is no path to exit
        ErrorMessage();
      else
        PrintPath(map, hConsole);

      DestroyMap(map);
    }

    //global error
    err = 0;
    printf("\n\n\n");
  }

  //memory leaks
  _CrtDumpMemoryLeaks();
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
  return 0;
}