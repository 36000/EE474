#include "Boolean.h"

static Bool evenCall = TRUE;
static Bool tempRising = FALSE;
static Bool sysComplete = FALSE;
static Bool diasComplete = FALSE;
static Bool pulseRising = FALSE;

unsigned int temperatureRaw = 0;
unsigned int systolicPressRaw = 0;
unsigned int diastolicPressRaw = 0;
unsigned int pulseRateRaw = 0;

void Measure();

void setup()
{
  // running on the uno - connect to tx1 and rx1 on the mega and to rx and tx on the uno
  // start serial port at 9600 bps and wait for serial port on the uno to open:
  Serial.begin(9600);
  evenCall = TRUE;
  tempRising = FALSE;
  sysComplete = FALSE;
  diasComplete = FALSE;
  pulseRising = FALSE;

  temperatureRaw = 75;
  systolicPressRaw = 80;
  diastolicPressRaw = 80;
  pulseRateRaw = 50;


}


unsigned long lastTime = 0;
void loop()
{
  char startOfMessage = 0x2A;
  char endOfMessage = 0x4B;
  unsigned long currentTime = millis();
  if (currentTime - lastTime > 5000) {
    Measure();
    lastTime = currentTime;
  }
  //  read incoming byte from the mega
  if (Serial.read() != startOfMessage) return;

  while (Serial.available() < 5);

  char taskIdentifier = Serial.read();
  char functionName = Serial.read();
  Serial.read(); // not needed
  Serial.read(); // not needed
  Serial.read();
  
  char data1 = 0;
  char data2 = 0;

  if (taskIdentifier == 1) {
    //temperature
    data1 = temperatureRaw;
  
  } else if (taskIdentifier == 2) {
    //pressure
    data1 = systolicPressRaw;
    data2 = diastolicPressRaw;
  } else {
    data1 = pulseRateRaw;
  }

  
  Serial.write(startOfMessage);
  Serial.write(taskIdentifier);
  Serial.write(functionName);
  Serial.write((char) data1);
  Serial.write((char) data2);
  Serial.write(endOfMessage);
}

void Measure () {

  if (tempRising) {
    if (evenCall) {
      temperatureRaw += 2;
      if (temperatureRaw > 50)
        tempRising = FALSE;
    } else {
      temperatureRaw -= 1;
    }
  } else {
    if (evenCall) {
      temperatureRaw -= 2;
      if (temperatureRaw < 15)
        tempRising = TRUE;
    } else {
      temperatureRaw += 1;
    }
  }

  if (diasComplete && sysComplete) {
    sysComplete = FALSE;
    diasComplete = FALSE;
    systolicPressRaw = 80;
    diastolicPressRaw = 80;
  }

  if (!sysComplete) {
    if (evenCall) {
      systolicPressRaw += 3;
      if (systolicPressRaw > 100)
        sysComplete = TRUE;
    } else {
      systolicPressRaw -= 1;
    }
  }

  if (!diasComplete) {
    if (evenCall) {
      diastolicPressRaw -= 2;
      if (diastolicPressRaw < 40)
        diasComplete = TRUE;
    } else {
      diastolicPressRaw += 1;
    }
  }

  if (pulseRising) {
    if (evenCall) {
      pulseRateRaw -= 1;
    } else {
      pulseRateRaw += 3;
      if (pulseRateRaw > 40)
        pulseRising = FALSE;
    }
  } else {
    if (evenCall) {
      pulseRateRaw += 1;
    } else {
      pulseRateRaw -= 3;
      if (pulseRateRaw < 15)
        pulseRising = TRUE;
    }
  }

  evenCall = 1 - evenCall;
}






