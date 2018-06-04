#include "DataStructs.h"
#include <arduino.h>
//#include <complex>
//#include "complex.h"
#include <stdlib.h>
#define MAX 200

using namespace std;

#include "arduinoFFT.h"

#define SAMPLES 256             //Must be a power of 2
#define SAMPLING_FREQUENCY 1000 //Hz, must be less than 10000 due to ADC

arduinoFFT FFT = arduinoFFT();

unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

void EKGProcess (void* data) {
  EKGData* eKGData = (EKGData*) data;

  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
  //SAMPLING
  for (int i = 0; i < SAMPLES; i++)
  {
    vReal[i] = (double)eKGData->EKGRawBuff[i];
    vImag[i] = 0;

    
    //approximate code for sampling in real time, but has hardware constraints
    /*
    microseconds = micros();    //Overflows after around 70 minutes!

    vReal[i] = analogRead(0);
    vImag[i] = 0;

    while (micros() < (microseconds + sampling_period_us)) {
    }
    */
  }

  //FFT
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

  EKGId++;
  EKGId = EKGId % 16;
  eKGData->EKGFreqBuff[EKGId] = (unsigned int) peak;
/*
  //PRINT RESULTS/
  //Serial.println(peak);     //Print out what frequency is the most dominant.

  for (int i = 0; i < (SAMPLES / 2); i++)
  {
    //View all these three lines in serial terminal to see which frequencies has which amplitudes/

    //Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
    //Serial.print(" ");
    Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins
  }
  */
}

