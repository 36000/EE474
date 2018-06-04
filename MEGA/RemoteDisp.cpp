#include "DataStructs.h"
#include <arduino.h>

void RemoteDisp (void* data) {
  RemoteDispData* remoteDispData = (RemoteDispData*) data;
  char values[4];
  
  Serial.println("\n\n\n\n");
  
  Serial.println("       Pocket Doctor (TM)");
  Serial.println("      Doctor's Name: Janet");
  Serial.println("      Patient's Name: John");
  Serial.println("    ------------------------");

  values[0] = (char) remoteDispData->tempCorrectedBuff[tCorrId * 3];
  values[1] = (char) remoteDispData->tempCorrectedBuff[tCorrId * 3 + 1];
  values[2] = '.';
  values[3] = (char) remoteDispData->tempCorrectedBuff[tCorrId * 3 + 2];
  Serial.print("    Temperature: "); for (int i = 0; i < 4; i++) Serial.print(values[i]); Serial.println(" C");
  for (int i = 0; i < 3; i++)
    values[i] = (char) remoteDispData->bloodPressCorrectedBuf[bp1CorrId * 3 + i];
  values[3] = ' ';
  Serial.print("    Systolic pressure: "); for (int i = 0; i < 4; i++) Serial.print(values[i]); Serial.println(" mm Hg");
  for (int i = 0; i < 3; i++)
    values[i] = (char) remoteDispData->bloodPressCorrectedBuf[bp2CorrId * 3 + i + 8 * 3];
  Serial.print("    Diastolic pressure: "); for (int i = 0; i < 4; i++) Serial.print(values[i]); Serial.println(" mm Hg");
  for (int i = 0; i < 3; i++)
    values[i] = (char) remoteDispData->pulseRateCorrectedBuf[prCorrId * 3 + i];
  Serial.print("    Pulse Rate: "); for (int i = 0; i < 4; i++) Serial.print(values[i]); Serial.println(" BPM");
  values[0] = values[1] = values[2] = values[3] = '0';
  Serial.print("    EKG: "); for (int i = 0; i < 4; i++) Serial.print(values[i]); Serial.println(" Hz");
  Serial.print("    Battery: "); if (*remoteDispData->batteryState < 100) Serial.print("0"); Serial.print(*remoteDispData->batteryState); Serial.println("/200");
  Serial.println(" ");
  Serial.print("Next Command: ");
}

