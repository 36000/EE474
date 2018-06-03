#include "DataStructs.h"
#include "Tasks.h"

#include <arduino.h>

unsigned long systemTimeBaseOld[1];

void Schedule(TCB* taskList) {
  systemTimeBase = (millis() - startingTime) / 1000;

  if (keypadFlag == FALSE && millis() > debounce + 300) {
    debounce = 0;
    keypadFlag = TRUE;
  }

  if (systemTimeBase > systemTimeBaseOld[0]) {
    systemTimeBaseOld[0] = systemTimeBase;
  } else {
    if (keypadFlag == TRUE)
      taskList[5].myTask(taskList[5].taskDataPtr);
    return;
  }

  if (displayFlag == TRUE)
    insert(&taskList[2], &llTCB);
  if (computeFlag == TRUE)
    insert(&taskList[1], &llTCB);
  if (measureFlag == TRUE){
    Serial.print('d');
    insert(&taskList[0], &llTCB);}
  if (warningAlarmFlag == TRUE)
    insert(&taskList[3], &llTCB);
  if (statusFlag == TRUE)
    insert(&taskList[4], &llTCB);
    
  TCB* current = llTCB.head;
  do {
    Serial.print('e');
    current->myTask(current->taskDataPtr);
    /*Serial.println(computeData.temperatureRawBuf[tRawId]);
    /Serial.println(computeData.bloodPressRawBuf[bp1RawId]);
    Serial.println(computeData.bloodPressRawBuf[bp2RawId + 8]);
    Serial.println(computeData.pulseRateRawBuf[prRawId]);
    Serial.println(computeData.tempCorrectedBuff[tCorrId * 3]);
    Serial.println(computeData.bloodPressCorrectedBuf[bp1CorrId * 3]);
    Serial.println(computeData.bloodPressCorrectedBuf[bp2CorrId * 3 + 8 * 3]);
    Serial.println(computeData.pulseRateCorrectedBuf[prCorrId * 3]);
    Serial.println(*computeData.measurementSelection);
    Serial.println();*/
  }
  while ((current = current->next) != NULL);
  Serial.println('l');
  
  if (measureFlag == TRUE) {
    del(&taskList[0], &llTCB);
    measureFlag = FALSE;
  }
  if (computeFlag == TRUE) {
    del(&taskList[1], &llTCB);
    computeFlag = FALSE;
  }
  if (displayFlag == TRUE) {
    del(&taskList[2], &llTCB);
  }
  if (warningAlarmFlag == TRUE) {
    del(&taskList[3], &llTCB);
    warningAlarmFlag = FALSE;
  }
  if (statusFlag == TRUE) {
    del(&taskList[4], &llTCB);
  }
}

void insert(TCB* node, TCB_ll* ll) {
  TCB* current = ll->head;
  while (current != NULL) {
    Serial.print('k');
    if (current == node) 
      return;
    current = current->next;
  }
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
    Serial.print('k');
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


