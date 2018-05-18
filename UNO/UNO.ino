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

//AudioFrequencyMeter meterp;
//AudioFrequencyMeter meterr;

void Measure();

void setup()
{
  // running on the uno - connect to tx1 and rx1 on the mega and to rx and tx on the uno
  // start serial port at 9600 bps and wait for serial port on the uno to open:
  Serial.begin(9600);

  /*meterp.setBandwidth(0, 4);
  meterp.begin(pinPulse, 20);  
  meterr.setBandwidth(0, 4);
  meterr.begin(pinResp, 20);  */
  
  tRawId, bp1RawId, bp2RawId, prRawId, rrRawId = 0;

  temperatureRawBuf[0] = 0;
  bloodPress1RawBuf[0] = 0;
  bloodPress2RawBuf[0] = 0;
  pulseRateRawBuf[0] = 0;
  respRateRawBuf[0] = 0;
}

typedef enum {NONE, TEMP, BLOOD1, BLOOD2, PULSE, RESP} dt;

int prhigh = 0;
int rrhigh = 0;
int prcount = 0;
int rrcount = 0;
float prfreq = 0;
float rrfreq = 0;

unsigned long lastTime = 0;
unsigned long lastFreqtime = 0;
void loop()
{
  unsigned long currentTime = millis();

  prhigh = sampleFreq(pinPulse, prhigh, &prcount);
  rrhigh = sampleFreq(pinResp, rrhigh, &rrcount);

  if (currentTime - lastFreqtime > 5000) {
    prcount = 0;
    rrcount = 0;
    prfreq = ((float) prcount) / 5;
    rrfreq = ((float) rrcount) / 5;
    lastFreqtime = currentTime;
  }
  
  if (currentTime - lastTime > 500) {
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

// range of 0.5 to 2 hertz
// range of 0.2 to 0.5 hertz
int sampleFreq(int pin, int high, int *count){
  float valf = analogRead(pin);
  if (high && valf < 200) {
    return 0;
  }

  if (!high && valf > 800) {
    (*count)++;
    return 1;
  }

  return high;
}

void Measure () {
  measureHelperTemp(pinTemp, temperatureRawBuf, &tRawId);
  measureHelper(pinPulse, pulseRateRawBuf, &prRawId, 0.5, 2, prfreq);
  measureHelper(pinResp, respRateRawBuf, &rrRawId, 0.2, 0.5, rrfreq);
}

void measureHelper(int pin, unsigned int* buf, unsigned int* index, float minvalue, float maxvalue, float freq) {
  unsigned int val = (freq - minvalue)*127/(maxvalue-minvalue);
  
  unsigned int dif;
  if (buf[*index] > val) 
    dif = (buf[*index] - val)*100/buf[*index];
  else
    dif = (val - buf[*index])*100/buf[*index];

  if (dif > 15) {
    (*index)++; (*index) %= 8;
    buf[*index] = val;
  }
}

void measureHelperTemp(int pin, unsigned int* buf, unsigned int* index) {
  float valf = analogRead(pin);

  valf/=8;

  unsigned int val = (unsigned int) valf;
  
  unsigned int dif;
  if (buf[*index] > val) 
    dif = (buf[*index] - val)*100/buf[*index];
  else
    dif = (val - buf[*index])*100/buf[*index];

  if (dif > 15) {
    (*index)++; (*index) %= 8;
    buf[*index] = val;
  }
}



