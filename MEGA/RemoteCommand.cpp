#include "DataStructs.h"
#include <arduino.h>

void RemoteCommand (void* data) {
  RemoteCommData* remoteCommandData = (RemoteCommData*) data;
  if (Serial.available() > 1) {
    switch((char) Serial.read()) { // I, S , P, D, M, W
      case 'I':
        break;
      case 'S':
        break;
      case 'P':
        break;
      case 'D':
        remoteDispFlag = TRUE;
        break;
      case 'M':
        break;
      case 'W':
        break;
      default:
        Serial.println("E");
        break;
    }
    Serial.print("Next Command: ");
  }
  
  while (Serial1.available() > 1) {
    Serial.read();
  }
}
