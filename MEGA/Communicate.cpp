#include "DataStructs.h"
#include <HardwareSerial.h>
#include <arduino.h>

#define START 0x2A
#define COMMUNICATE_ID 10
#define END 0x4B

int isCuffReady = 0;

void Communicate(void* data) {
  CommunicateData* communicateData = (CommunicateData*) data;

  TCB* current = llData.head;
  do {
    char select = (char) *(dt*)current->taskDataPtr;
  
    //Serial.println('A');
    Serial1.write(START);
    Serial1.write(select);
    Serial1.write(COMMUNICATE_ID);
    Serial1.write('\0'); // no data required
    Serial1.write(END);
  
    while (Serial1.available() < 7);
  
    Serial1.read();
    
    cuffInflation = Serial1.read();
    if (cuffInflation >= 8) {
      if(isCuffReady == 0) {
        communicateData->bloodPressRawBuf[bp1RawId] = (unsigned int) Serial1.read();
        delay(5);
        communicateData->bloodPressRawBuf[bp2RawId + 8] = (unsigned int) Serial1.read();
        delay(5);
      } else {
        Serial1.read();
        Serial1.read();
      }
      isCuffReady = 1;
    } else {
      Serial1.read();
      Serial1.read();
      isCuffReady = 0;
    }
  
    Serial1.read();
  
    switch (*communicateData->measurementSelection) {
      case NONE:
        Serial1.read();
        break;
      case BLOOD1:
        Serial1.read();
        break;
      case BLOOD2:
        Serial1.read();
        break;
      case TEMP:
        communicateData->temperatureRawBuf[tRawId] = (unsigned int) Serial1.read();
        //Serial.println(communicateData->temperatureRawBuf[tRawId]);
        break;
      case PULSE:
        communicateData->pulseRateRawBuf[prRawId] = (unsigned int) Serial1.read();
        break; 
      case RESP:
        communicateData->respRateRawBuf[rrRawId] = (unsigned int) Serial1.read();
        break;  
      default:
        Serial1.read();
        break;
    }
    
    if (Serial1.read() != END)
      Serial.print("Message Validation Error");
  }  while((current = current->next) != NULL);
}


