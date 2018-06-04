#include "DataStructs.h"
#include <arduino.h>

#define FREQ_PIN 15
// Generates sinusoidal signal for EKG process to perform FFT of

double sine(double freq, double t) {
  Serial.print("s-");
  return sin(t*freq*2.0*PI);
}

void EKGCapture (void* data) {
  Serial.println();
  Serial.println("yeet");
  EKGData* ekgData = (EKGData*) data;
  Serial.print("Frequency: ");
  Serial.println(analogRead(FREQ_PIN));
  double freq = (double) map(analogRead(FREQ_PIN), 0, 1023, 35, 3750); // Hz
  Serial.println(freq);
  for(int i = 0; i < 256; i++){
    double t = ((double) i) / (freq * 10 * EULER + 1);
    Serial.print("t: ");
    Serial.println(t);
    ekgData->EKGRawBuff[i] = sine(freq, t);
    Serial.println();
    Serial.print(ekgData->EKGRawBuff[i]);
    Serial.println();

    /*
    while(millis() - currentTime < (1000/SAMPLING_FREQUENCY)
    {
      delay(1);  
    }
    float valf = analogRead(15);  //should read from pin 15
    unsigned int val = (unsigned int) valf;
    eKGData->EKGRawBuff[i] = val;
    */
  }
  taskFlags[9] = TRUE;
}





























