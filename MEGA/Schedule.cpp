#include "DataStructs.h"
#include "Tasks.h"

#include <arduino.h>

unsigned long systemTimeBaseOld[1];

void Schedule(TCB* taskList) {
  systemTimeBase = (millis() - startingTime) / 500;

  if (taskFlags[5] == FALSE && millis() > debounce + 250) {
    debounce = 0;
    taskFlags[5] = TRUE;
  }

  if (taskFlags[5] == TRUE)
    taskList[5].myTask(taskList[5].taskDataPtr);

  if (systemTimeBase > systemTimeBaseOld[0])
    systemTimeBaseOld[0] = systemTimeBase;
  else
    return;

  // turn on certain tasks no matter what
  taskFlags[0] = taskFlags[1] = taskFlags[2] = taskFlags[3] = taskFlags[4] = taskFlags[7] = TRUE;

  for (int i = 0; i < taskNumber; i++)
    if (taskFlags[i] == TRUE)
      insert(&taskList[i], &llTCB);
    
  TCB* current = llTCB.head;
  do {
    current->myTask(current->taskDataPtr);
  }
  while ((current = current->next) != NULL);

  for (int i = 0; i < taskNumber; i++) {
    del(&taskList[i], &llTCB);
    taskFlags[i] = FALSE;
  }
}

bool check(TCB* node, TCB_ll* ll) {
  TCB* current = ll->head;
  while (current != NULL) {
    if (current == node) 
      return true;
    current = current->next;
  }
  return false;
}

void insert(TCB* node, TCB_ll* ll) {
  if(check(node, ll))
    return;
  if (ll->tail == NULL || ll->head == NULL) {
    ll->head = node;
    ll->tail = node;
  }
  else {
    ll->tail->next = node;
    node->prev = ll->tail;
    ll->tail = node;
  }
}

void del(TCB* node, TCB_ll* ll) {
  TCB* current = ll->head;
  while (current != NULL) {
    if (current == node) 
      break;
    current = current->next;
  }

  if (current == NULL)
    return;

  if (current == ll->tail)
    ll->tail = current->prev;
  else
    current->next->prev = current->prev;

  if (current == ll->head)
    ll->head = current->next;
  else
    current->prev->next = current->next;

  current->prev = NULL;
  current->next = NULL;
}


