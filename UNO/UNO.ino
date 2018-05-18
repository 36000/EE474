#include "Boolean.h"

unsigned int tRawId, bp1RawId, bp2RawId, prRawId, rrRawId;

static unsigned int temperatureRawBuf[8];
static unsigned int bloodPress1RawBuf[8];
static unsigned int bloodPress2RawBuf[8];
static unsigned int pulseRateRawBuf[8];
static unsigned int respRateRawBuf[8];

const char startOfMessage = 0x2A;
const char endOfMessage = 0x4B;

const char pinTemp = 3;
const char pinPulse = 0;
const char pinResp = 1;
// 4 is cuff bulb, 5 is cuff switcher

void Measure();

void setup()
{
  // running on the uno - connect to tx1 and rx1 on the mega and to rx and tx on the uno
  // start serial port at 9600 bps and wait for serial port on the uno to open:
  Serial.begin(9600);
  
  tRawId, bp1RawId, bp2RawId, prRawId, rrRawId = 0;

  temperatureRawBuf[0] = 0;
  bloodPress1RawBuf[0] = 0;
  bloodPress2RawBuf[0] = 0;
  pulseRateRawBuf[0] = 0;
  respRateRawBuf[0] = 0;
}

typedef enum {NONE, TEMP, BLOOD1, BLOOD2, PULSE, RESP} dt;

unsigned long lastTime = 0;
void loop()
{
  unsigned long currentTime = millis();
  if (currentTime - lastTime > 5000) {
    Measure();
    lastTime = currentTime;
  }
  
  //  read incoming byte from the mega
  if (Serial.read() != startOfMessage) return;
  while (Serial.available() < 4);
  char taskIdentifier = Serial.read();
  char functionName = Serial.read();
  Serial.read(); // not needed
  Serial.read(); // not needed
  Serial.read();
  
  char data = 0;
  switch (taskIdentifier) { //NONE, TEMP, BLOOD1, BLOOD2, PULSE, RESP
    case TEMP:
      data = temperatureRawBuf[tRawId];
      break;
    case BLOOD1:
      data = bloodPress1RawBuf[bp1RawId];
      break;
    case BLOOD2:
      data = bloodPress2RawBuf[bp2RawId];
      break;
    case PULSE:
      data = pulseRateRawBuf[prRawId];
      break;
    case RESP:
      data = respRateRawBuf[rrRawId];
  }
  
  Serial.write(startOfMessage);
  Serial.write(taskIdentifier);
  Serial.write(functionName);
  Serial.write((char) data);
  Serial.write(endOfMessage);
}

void Measure () {
  measureHelper(pinTemp, temperatureRawBuf, &tRawId, 350, 380);
  measureHelper(pinPulse, pulseRateRawBuf, &prRawId, 10, 200);
  measureHelper(pinResp, respRateRawBuf, &rrRawId, 10, 50);
}

void measureHelper(int pin, unsigned int* buf, unsigned int* index, int minScale, int maxScale) {
  unsigned int val = analogRead(pin);
  
  val = val*(maxScale - minScale)/1023 + minScale;

  unsigned int dif;
  if (buf[*index] > val) 
    dif = (buf[*index] - val)*100/buf[*index];
  else
    dif = (val - buf[*index])*100/buf[*index];

  if (dif > 15) {
    *index++;
    buf[*index] = val;
  }
}



