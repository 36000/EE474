#include "DataStructs.h"
#include <arduino.h>

void RemoteDisp (void* data) {
  RemoteDispData* remoteDispData = (RemoteDispData*) data;
  char values[4];

  if (systemTimeBase % 10 != 0)
    return;
  
  Serial.println("\n\n\n\n");
  
  Serial.println("   Pocket Doctor (TM)");
  Serial.println("  Doctor's Name: Janet");
  Serial.println("  Patient's Name: John");
  Serial.println("------------------------");

  values[0] = (char) remoteDispData->tempCorrectedBuff[tCorrId * 3];
  values[1] = (char) remoteDispData->tempCorrectedBuff[tCorrId * 3 + 1];
  values[2] = '.';
  values[3] = (char) remoteDispData->tempCorrectedBuff[tCorrId * 3 + 2];
  Serial.print("Temperature: "); for (int i = 0; i < 4; i++) Serial.print(values[i]); Serial.println(" C");
  /*Systolic pressure: <systolic pressure> mm Hg
  Diastolic pressure: <diastolic pressure> mm Hg
  Pulse rate: <pulse rate> BPM
  EKG < Measured Frequency reading > Hz
  Battery: <charge remaining> */
}

