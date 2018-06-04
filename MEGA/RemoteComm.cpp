#include "DataStructs.h"
#include <arduino.h>

void RemoteComm (void* data) {
  RemoteCommData* remoteCommData = (RemoteCommData*) data;

  Bool anyWarning = FALSE;
  Bool anyAlarm2 = FALSE;
  
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
      if(batteryDead)
        batteryDead = FALSE;
      else
        batteryDead = TRUE;
      break;
    case W:
      Serial.println(" ");
      Serial.print("WARNINGS: ");
      if (tempHigh) {
        Serial.print("TEMPERATURE, ");
        anyWarning = TRUE;
      }
      if (bpHigh1 || bpHigh2) {
        Serial.print("PRESSURE, ");
        anyWarning = TRUE;
      }
    
      if (pulseLow) {
        Serial.print("PULSE RATE, ");
        anyWarning = TRUE;
      }
      if (anyWarning == FALSE) {
        Serial.print("NONE");
      }
      Serial.println(" ");
      Serial.print("ALARMS: ");
      if (tempOutOfRange == 1) {
        Serial.print("TEMPERATURE, ");
        anyAlarm2 = TRUE;
      }
      if (bpOutOfRange == 1) {
        Serial.print("PRESSURE, ");
        anyAlarm2 = TRUE;
      }
    
      if (pulseOutOfRange == 1) {
        Serial.print("PULSE RATE, ");
        anyAlarm2 = TRUE;
      }
      if (anyAlarm2 == FALSE) {
        Serial.print("NONE");
      }
      Serial.println(" ");
      break;
    default:
      return;
  }
  if (commSig != I)
    Serial.println("Task Completed");
  Serial.print("Next Command: ");
}
