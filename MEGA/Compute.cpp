#include "DataStructs.h"
#include <arduino.h>

void itoa(unsigned char* a, unsigned int i, unsigned int index) {
  a[index] = 48 + i / 100;
  a[index + 1] = 48 + (i / 10) % 10;
  a[index + 2] = 48 + i % 10;
}

void Compute (void* data) {
  ComputeData* computeData = (ComputeData*) data;

  TCB* current = llData.head;
  while (current != NULL) {
    switch (*(dt*)current->taskDataPtr) {
      case NONE:
        return;
      case TEMP:
        tCorrId++; tCorrId %= 8;
        itoa(computeData->tempCorrectedBuff, (unsigned int) (computeData->temperatureRawBuf[tRawId]*(500 - 250)/128 + 250), tCorrId * 3);
        return;
      case BLOOD1:
        bp1CorrId++; bp1CorrId %= 8;
        itoa(computeData->bloodPressCorrectedBuf, (unsigned int) (computeData->bloodPressRawBuf[bp1RawId]*(150 - 100)/128 + 100), bp1CorrId * 3);
        return;
      case BLOOD2:
        bp2CorrId++; bp2CorrId %= 8;
        itoa(computeData->bloodPressCorrectedBuf, (unsigned int) (computeData->bloodPressRawBuf[8 + bp2RawId]*(100 - 50)/128 + 50), bp2CorrId * 3 + 8 * 3);
        return;
      case PULSE:
        prCorrId++; prCorrId %= 8;
        itoa(computeData->pulseRateCorrectedBuf, computeData->pulseRateRawBuf[prRawId]*12, prCorrId * 3);   
        return;    
      case RESP:
        rrCorrId++; rrCorrId %= 8;
        itoa(computeData->respRateCorrectedBuf, computeData->respRateRawBuf[rrRawId]*12, rrCorrId * 3);   
        return;
      default:
        return;
    }
    current = current->next;
  }
}

