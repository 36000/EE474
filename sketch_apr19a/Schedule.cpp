#include "DataStructs.h"
#include "Tasks.h"

#include <arduino.h>

unsigned long systemTimeBaseOld[1];

void Schedule(TCB* taskList) {
  unsigned long currentTime = millis() - startingTime;

  if (currentTime % 1000 != 0)
    return;
  
  systemTimeBase = currentTime / 1000;

  if (systemTimeBase == systemTimeBaseOld[0])
    return;

  systemTimeBaseOld[0] = systemTimeBase;

  // for testing

  *computeData.measurementSelection = systemTimeBase % 4;
  if (systemTimeBase % 8 == 0) {
    alarmAcknowledge = TRUE;
  } else {
    alarmAcknowledge = FALSE;
  }

  // for testing

  if (systemTimeBase % 5 == 0 && measurementSelection != NONE) {
    insert(&taskList[0]);
    insert(&taskList[1]);
    insert(&taskList[4]);
  }

  TCB* current = ll.head;

  do {
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

  if (systemTimeBase % 5 == 0 && measurementSelection != NONE) {
    del(&taskList[0]);
    del(&taskList[1]);
    del(&taskList[4]);
    measurementSelection = NONE;
  }
}

void insert(TCB* node) {
  if (ll.tail == NULL) {
    ll.head = node;
    ll.tail = node;
  }
  else {
    ll.tail->next = node;
    node->prev = ll.tail;
    ll.tail = node;
  }
}

void del(TCB* node) {
  TCB* current = ll.head;
  while (current != NULL) {
    current = current->next;
    if (current == node) 
      break;
  }

  if (current == NULL)
    return;

  if (current == ll.tail)
    ll.tail = current->prev;
  else
    current->next->prev = current->prev;

  if (current == ll.head)
    ll.head = current->next;
  else
    current->prev->next = current->next;
}


