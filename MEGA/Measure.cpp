#include "DataStructs.h"
#include <arduino.h>

void Measure (void* data) {
  MeasureData* measureData = (MeasureData*) data;
  TCB* current = llData.head;
  while(current != NULL) {
    switch (*(dt*)current->taskDataPtr) {
      case NONE:
        //Serial.print(*(dt*)current->taskDataPtr);
        break;
      case BLOOD1:
        break;
      case BLOOD2:
        break;
      case TEMP:
        tRawId++; tRawId %= 8;
        break;
      case PULSE:
        prRawId++; prRawId %= 8;
        break;
      case RESP:
        rrRawId++; rrRawId %= 8;
        break;
      default:
        //Serial.print(*(dt*)current->taskDataPtr);
        return;
    }
    current = current->next;
  }

  Communicate(data);
}
