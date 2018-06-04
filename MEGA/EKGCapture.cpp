#include "DataStructs.h"
#include <arduino.h>

unsigned long DELAY_BETWEEN_SAMPLES = 50;

void EKGCapture (void* data) {
  EKGData* eKGData = (EKGData*) data;
  unsigned long currentTime = millis();
  for(int i = 0; i++; i < 256){
    while(millis() - currentTime < DELAY_BETWEEN_SAMPLES)
    {
      delay(1);  
    }
    float valf = analogRead(15);  //should read from pin 15
    unsigned int val = (unsigned int) valf;
    eKGData->EKGRawBuff[i] = val;
  }
  
}





























