#include "DataStructs.h"
#include <HardwareSerial.h>

#define START 0x2A
#define COMMUNICATE_ID 10
#define END 0x4B

int isCuffReady = 0;

void Communicate(void* data) {
  CommunicateData* communicateData = (CommunicateData*) data;
  
  char select = (char) *communicateData->measurementSelection;

  //Serial.println('A');
  Serial1.write(START);
  Serial1.write(select);
  Serial1.write(COMMUNICATE_ID);
  Serial1.write('\0'); // no data required
  Serial1.write(END);

  while (Serial1.available() < 5);

  Serial1.read();
  
  cuffInflation = Serial1.read();
  if (cuffInflation >= 8) {
    isCuffReady = 1;
  } else {
    isCuffReady = 0;
  }

  Serial1.read();

  switch (*communicateData->measurementSelection) {
    case NONE:
      Serial1.read();
      break;
    case BLOOD1:
      if(isCuffReady)
        communicateData->bloodPressRawBuf[bp1RawId] = (unsigned int) Serial1.read();
      else
        Serial1.read();
      break;
    case BLOOD2:
      if(isCuffReady)
        communicateData->bloodPressRawBuf[bp2RawId + 8] = (unsigned int) Serial1.read();
      else
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
  
  Serial1.read();
}


