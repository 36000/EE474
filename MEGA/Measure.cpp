#include "DataStructs.h"

void Measure (void* data) {
  MeasureData* measureData = (MeasureData*) data;

  switch (*measureData->measurementSelection) {
    case NONE:
      return;
    case BLOOD1:
      bp1RawId++; bp1RawId %= 8;
      break;
    case BLOOD2:
      bp2RawId++; bp2RawId %= 8;
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
      return;
  }

  Communicate(data);
}
