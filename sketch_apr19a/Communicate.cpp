#include "DataStructs.h"
#include <HardwareSerial.h>

#define START 0x2A
#define COMMUNICATE_ID 10
#define END 0x4B


void Communicate(void* data) {
  CommunicateData* communicateData = (CommunicateData*) data;
  
  char select = (char) *communicateData->measurementSelection;

  //Serial.println('A');
  Serial1.write(START);
  Serial1.write(select);
  Serial1.write(COMMUNICATE_ID);
  Serial1.write('\0'); // no data required
  Serial1.write('\0'); // no data required
  Serial1.write(END);

  while (Serial1.available() < 6);

  Serial1.read();
  Serial1.read();
  Serial1.read();

  switch (*communicateData->measurementSelection) {
    case NONE:
      return;
    case BLOOD1:
      communicateData->bloodPressRawBuf[bp1RawId] = (unsigned int) Serial1.read();
      break;
    case BLOOD2:
      communicateData->bloodPressRawBuf[bp2RawId + 8] = (unsigned int) Serial1.read();
      break;
    case TEMP:
      communicateData->temperatureRawBuf[tRawId] = (unsigned int) Serial1.read();
      //Serial.println(communicateData->temperatureRawBuf[tRawId]);
      break;
    case PULSE:
      communicateData->pulseRateRawBuf[prRawId] = (unsigned int) Serial1.read();
      break;   
    default:
      return;
  }

  Serial1.read();
  Serial1.read();
}


