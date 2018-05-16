#include "DataStructs.h"

void Status (void* data) {
  StatusData* statusData = (StatusData*) data;

  *statusData->batteryState -= 1;
}
