#include "DataStructs.h"
#include <arduino.h>

void RemoteComm (void* data) {
  RemoteCommData* remoteCommData = (RemoteCommData*) data;
  
  switch(commSig) {
    case I:
      break;
    case S:
      for (int i = 1; i < 6; i++)
        insert(&dataList[i], &llData);
      break;
    case P:
      llData.head = NULL;
      break;
    case M:
    case W:
    default:
      return;
  }

  Serial.print("Next Command: ");
}
