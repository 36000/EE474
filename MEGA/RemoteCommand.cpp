#include "DataStructs.h"
#include <arduino.h>

void RemoteCommand (void* data) {
  RemoteCommData* remoteCommandData = (RemoteCommData*) data;
  if (Serial.available() > 0) {
    //Serial.println(" ");
    switch((char) Serial.read()) { // I, S , P, D, M, W
      case 'I':
        commSig = I;
        taskFlags[10] = TRUE;
        break;
      case 'S':
        commSig = S;
        taskFlags[10] = TRUE;
        break;
      case 'P':
        commSig = P;
        taskFlags[10] = TRUE;
        break;
      case 'D':
        commSig = M;
        taskFlags[10] = TRUE;
        break;
      case 'M':
        taskFlags[6] = TRUE;
        break;
      case 'W':
        commSig = W;
        taskFlags[10] = TRUE;
        break;
      default:
        Serial.println("E");
        Serial.print("Next Command: ");
        break;
    }
  }
  
  while (Serial1.available() > 0) {
    Serial.read();
  }
}
