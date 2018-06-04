#include "DataStructs.h"
#include <arduino.h>

void RemoteCommand (void* data) {
  RemoteCommData* remoteCommandData = (RemoteCommData*) data;
  if (Serial.available() > 1) {
    //Serial.println(" ");
    switch((char) Serial.read()) { // I, S , P, D, M, W
      case 'I':
        Serial.print("Next Command: ");
        break;
      case 'S':
        break;
      case 'P':
        break;
      case 'D':
        taskFlags[6] = TRUE;
        break;
      case 'M':
        break;
      case 'W':
        break;
      default:
        Serial.println("E");
        Serial.print("Next Command: ");
        break;
    }
  }
  
  while (Serial1.available() > 1) {
    Serial.read();
  }
}
