#include "DataStructs.h"

void Status (void* data) {
  StatusData* statusData = (StatusData*) data;

  if (systemTimeBase % 10 == 0)
    *statusData->batteryState -= 1;
}
