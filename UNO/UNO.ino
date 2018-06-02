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

  attachInterrupt(pinPulse, incrementPulse, RISING);
  attachInterrupt(respPulse, incrementResp, RISING);

  tRawId, bp1RawId, bp2RawId, prRawId, rrRawId = 0;

  temperatureRawBuf[0] = 0;
  bloodPress1RawBuf[0] = 0;
  bloodPress2RawBuf[0] = 0;
  pulseRateRawBuf[0] = 0;
  respRateRawBuf[0] = 0;
}

void incrementPulse() {
  prcount++;
}

void incrementResp() {
  rrcount++;
}

typedef enum {NONE, TEMP, BLOOD1, BLOOD2, PULSE, RESP} dt;

int prcount = 0;
int rrcount = 0;
int prfreq = 0;
int rrfreq = 0;

unsigned char cuffInflation;
unsigned long lastTime = 0;
unsigned long lastFreqtime = 0;
void loop()
{
  unsigned long currentTime = millis();

  if (currentTime - lastFreqtime > 5000) {
    prfreq = prcount;
    rrfreq = rrcount;
    prcount = 0;
    rrcount = 0;
    lastFreqtime = currentTime;
  }

  if (currentTime - lastTime > 500) {
    Measure();
    //Serial.println(cuffInflation);
    lastTime = currentTime;
  }

  //  read incoming byte from the mega
  if (Serial.read() != startOfMessage) return;
  while (Serial.available() < 4);
  char taskIdentifier = Serial.read();
  char functionName = Serial.read();
  Serial.read(); // not needed
  Serial.read(); // not needed
  if (Serial1.read() != endOfMessage)
    Serial.print("Message Validation Error");

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
  Serial.write(cuffInflation);
  Serial.write((char)bloodPress1RawBuf[bp1RawId]);
  Serial.write((char)bloodPress2RawBuf[bp2RawId]);
  Serial.write(functionName);
  Serial.write((char) data);
  Serial.write(endOfMessage);
}

void Measure () {
  float cuffButton = analogRead(4);
  float cuffSwitch = analogRead(5);
  if (cuffButton < 200) {
    if ((cuffSwitch < 200) && (cuffInflation > 0))
      cuffInflation--;
    else if ((cuffSwitch >= 200) && (cuffInflation < 10))
      cuffInflation++;
  }

  measureHelper(pinTemp, temperatureRawBuf, &tRawId);
  measureHelper(pinTemp, bloodPress1RawBuf, &bp1RawId);
  measureHelper(pinTemp, bloodPress2RawBuf, &bp2RawId);
  measureHelperRate(pinPulse, pulseRateRawBuf, &prRawId, prfreq);
  measureHelperRate(pinResp, respRateRawBuf, &rrRawId, rrfreq);
}

void measureHelperRate(int pin, unsigned int* buf, unsigned int* index, float freq) {
  unsigned int val = freq;

  if (val > 16)
    val = 16;

  unsigned int dif;
  if (buf[*index] > val)
    dif = (buf[*index] - val) * 100 / buf[*index];
  else
    dif = (val - buf[*index]) * 100 / buf[*index];

  if (dif > 15) {
    (*index)++; (*index) %= 8;
    buf[*index] = val;
  }
}

void measureHelper(int pin, unsigned int* buf, unsigned int* index) {
  float valf = analogRead(pin);

  valf /= 8;

  unsigned int val = (unsigned int) valf;

  unsigned int dif;
  if (buf[*index] > val)
    dif = (buf[*index] - val) * 100 / buf[*index];
  else
    dif = (val - buf[*index]) * 100 / buf[*index];

  if (dif > 15) {
    (*index)++; (*index) %= 8;
    buf[*index] = val;
  }
}


