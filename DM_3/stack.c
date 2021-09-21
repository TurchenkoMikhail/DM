#include "Header.h"

stack_t stack; //global stack

//add to stack
void StackPush(point_t p) {

  elem_t* newelem = (elem_t*)malloc(sizeof(elem_t));
  if (!newelem)
    return;

  newelem->point = p;
  newelem->next = NULL;
  newelem->prev = NULL;

  if (!stack.last) { //stack is empty
    stack.first = newelem;
    stack.last = newelem;
  }
  else {
    newelem->prev = stack.last;
    stack.last->next = newelem;
    stack.last = stack.last->next;
  }
}

bool StackIsEmpty(){
  return stack.last ? false : true;
}

//look at top element without removing it
point_t StackTop() {
  return stack.last->point;
}

//remove last element without bringing it
void StackPop() {
  if (stack.last == NULL)
    return; //stack is empty
  
  else if (stack.last->prev) {
    stack.last = stack.last->prev;
    free(stack.last->next);
    stack.last->next = NULL;
  }
  else {
    free(stack.last);
    stack.last = NULL;
    stack.first = NULL;
  }
}

void DestroyStack() {
  while (!StackIsEmpty())
    StackPop();
}