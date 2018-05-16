#include "DataStructs.h"

void itoa(unsigned char* a, unsigned int i, unsigned int index) {
  a[index] = 48 + i / 100;
  a[index + 1] = 48 + (i / 10) % 10;
  a[index + 2] = 48 + i % 10;
}

void Compute (void* data) {
  ComputeData* computeData = (ComputeData*) data;

  switch (*computeData->measurementSelection) {
    case NONE:
      return;
    case TEMP:
      tCorrId++; tCorrId %= 8;
      itoa(computeData->tempCorrectedBuff, 50 + (unsigned int) (7.5 * computeData->temperatureRawBuf[tRawId]), tCorrId * 3);
      return;
    case BLOOD:
      bp1CorrId++; bp1CorrId %= 8;
      itoa(computeData->bloodPressCorrectedBuf, 9 + 2 * computeData->bloodPressRawBuf[bp1RawId], bp1CorrId * 3);
    
      bp2CorrId++; bp2CorrId %= 8;
      itoa(computeData->bloodPressCorrectedBuf, 6 + (unsigned int) (1.5 * computeData->bloodPressRawBuf[8 + bp2RawId]), bp2CorrId * 3 + 8 * 3);
      return;
    case PULSE:
      prCorrId++; prCorrId %= 8;
      itoa(computeData->pulseRateCorrectedBuf, 8 + 3 * computeData->pulseRateRawBuf[prRawId], prCorrId * 3);   
      return;    
    default:
      return;
  }
}

