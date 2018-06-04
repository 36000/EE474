#include "DataStructs.h"
#include <arduino.h>

void RemoteComm (void* data) {
  RemoteCommData* remoteCommData = (RemoteCommData*) data;
  
  switch(commSig) {
    case I:
    case S:
    case P:
    case M:
    case W:
    return;
  }
}
