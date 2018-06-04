#include "DataStructs.h"
#include <arduino.h>
//#include <complex>
//#include "complex.h"
#include <stdlib.h>
#define MAX 200

using namespace std;

void EKGProcess (void* data) {
  EKGData* eKGData = (EKGData*) data; 
/*
   int n = 256;  
  double samplingStep = 0.05; // should be brought into question
  
  complex<double> vec[MAX];
  cout << "specify the array" << endl;
  for(int i = 0; i < n; i++) {
    vec[i] = eKGData->EKGRawBuff[i];
  }
  FFT(vec, n, samplingStep);
  
  double frequency = 0.0;
  double returnVal = 0.0;
  double highestMagnitude = 0.0;
  for(int j = 0; j < n; j++)
    frequency += samplingStep;
    double currentMag = vec[j];
    cout << vec[j] << endl;*/
  
}

/*
 * double frequency
 * double returnval
 * for the first half of the array{
 * double + (some small value)
 *  magnitude = first part of vector ^2 + second part of vector ^2
 *  if (magnitude is the greates magnitude seen so far)
 *    returnval = frequency
 * 
 * 
 * }
 * 
 * 
 */


#include "arduinoFFT.h"
 
#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 1000 //Hz, must be less than 10000 due to ADC
 
arduinoFFT FFT = arduinoFFT();
 
unsigned int sampling_period_us;
unsigned long microseconds;
 
double vReal[SAMPLES];
double vImag[SAMPLES];
 
void setup1() {
    Serial.begin(115200);
 
    sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
}
 
void loop1() {
   
    //SAMPLING
    for(int i=0; i<SAMPLES; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!
     
        vReal[i] = analogRead(0);
        vImag[i] = 0;
     
        while(micros() < (microseconds + sampling_period_us)){
        }
    }
 
    //FFT
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
 
    //PRINT RESULTS/
    //Serial.println(peak);     //Print out what frequency is the most dominant.
 
    for(int i=0; i<(SAMPLES/2); i++)
    {
        //View all these three lines in serial terminal to see which frequencies has which amplitudes/
         
        //Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
        //Serial.print(" ");
        Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins
    }
 
    //delay(1000);  //Repeat the process every second OR:
    while(1);       //Run code once
}

