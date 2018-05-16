extern "C" {
#include "DataStructs.h"
#include "Tasks.h"
}

#include "Elegoo.h"

#include <arduino.h>
#include <stdio.h>
#include <stdlib.h>

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
//TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
//Elegoo_GFX_Button buttons[5];

//char buttonlabels[5][MAX_STRING_SIZE] = {"Select Data", "Warnings O/F", "Temp", "Blood Pres.", "Pulse Rate" };
//uint16_t buttoncolors[5] = {ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE};
                             


//These functions require display access so they are cpp
//They are declared separately from the other functions
void Display (void* data);
void Keypad(void* data);

#include "DataStructs.h"

#include <arduino.h>

unsigned long systemTimeBaseOld[1];

void Schedule(TCB* taskList) {
  unsigned long currentTime = millis() - startingTime;

  if (currentTime % 1000 != 0)
    return;
  
  systemTimeBase = currentTime / 1000;

  if (systemTimeBase == systemTimeBaseOld[0])
    return;

  systemTimeBaseOld[0] = systemTimeBase;

  // for testing

  *computeData.measurementSelection = systemTimeBase % 4;
  if (systemTimeBase % 8 == 0) {
    alarmAcknowledge = TRUE;
  } else {
    alarmAcknowledge = FALSE;
  }

  // for testing

  if (systemTimeBase % 5 == 0 && measurementSelection != NONE) {
    insert(&taskList[0]);
    insert(&taskList[1]);
    insert(&taskList[4]);
  }

  TCB* current = ll.head;

  do {
    current->myTask(current->taskDataPtr);
    /*Serial.println(computeData.temperatureRawBuf[tRawId]);
    /Serial.println(computeData.bloodPressRawBuf[bp1RawId]);
    Serial.println(computeData.bloodPressRawBuf[bp2RawId + 8]);
    Serial.println(computeData.pulseRateRawBuf[prRawId]);
    Serial.println(computeData.tempCorrectedBuff[tCorrId * 3]);
    Serial.println(computeData.bloodPressCorrectedBuf[bp1CorrId * 3]);
    Serial.println(computeData.bloodPressCorrectedBuf[bp2CorrId * 3 + 8 * 3]);
    Serial.println(computeData.pulseRateCorrectedBuf[prCorrId * 3]);
    Serial.println(*computeData.measurementSelection);
    Serial.println();*/
  }
  while ((current = current->next) != NULL);

  if (systemTimeBase % 5 == 0 && measurementSelection != NONE) {
    del(&taskList[0]);
    del(&taskList[1]);
    del(&taskList[4]);
    measurementSelection = NONE;
  }
}

void insert(TCB* node) {
  if (ll.tail == NULL) {
    ll.head = node;
    ll.tail = node;
  }
  else {
    ll.tail->next = node;
    node->prev = ll.tail;
    ll.tail = node;
  }
}

void del(TCB* node) {
  TCB* current = ll.head;
  while (current != NULL) {
    current = current->next;
    if (current == node) 
      break;
  }

  if (current == NULL)
    return;

  if (current == ll.tail)
    ll.tail = current->prev;
  else
    current->next->prev = current->prev;

  if (current == ll.head)
    ll.head = current->next;
  else
    current->prev->next = current->next;
}




// warning / alarms
Bool bpHigh;
Bool tempHigh;
Bool pulseLow;
Bool batteryLow;
Bool batteryDead;

TCB_ll ll;

unsigned char bpOutOfRange;
unsigned char tempOutOfRange;
unsigned char pulseOutOfRange;

unsigned long systemTimeBase;
unsigned long startingTime;

static TCB taskList[6];

unsigned int tRawId, bp1RawId, bp2RawId, prRawId, tCorrId, bp1CorrId, bp2CorrId, prCorrId;

static unsigned int temperatureRawBuf[8];
static unsigned int bloodPressRawBuf[16];
static unsigned int pulseRateRawBuf[8];

static unsigned char tempCorrectedBuff[8 * 3];
static unsigned char bloodPressCorrectedBuf[16 * 3];
static unsigned char pulseRateCorrectedBuf[8 * 3];

static unsigned short batteryState;

dt measurementSelection;

Bool alarmAcknowledge;

static MeasureData measureData;
static CommunicateData communicateData;
ComputeData computeData; // static , removed for temporary
static DisplayData displayData;
static WarningAlarmData warningAlarmData;
static StatusData statusData;
static KeypadData keypadData;

void ElegooSetup();

void setup() {
  ElegooSetup();

  Serial1.begin(9600);
  Serial.begin(9600);
  ll.head = NULL;
  ll.tail = NULL;

  startingTime = millis();
  systemTimeBase = 0;

  bpHigh = FALSE;
  tempHigh = FALSE;
  pulseLow = FALSE;
  batteryLow = FALSE;
  batteryDead = FALSE;

  bpOutOfRange = 0;
  tempOutOfRange = 0;
  pulseOutOfRange = 0;

  tRawId = bp1RawId = bp2RawId = prRawId = tCorrId = bp1CorrId = bp2CorrId = prCorrId = 0;

  temperatureRawBuf[0] = 75;
  bloodPressRawBuf[0] = 80;
  bloodPressRawBuf[8] = 80;
  pulseRateRawBuf[0] = 50;

  tempCorrectedBuff[0] = tempCorrectedBuff[1] = tempCorrectedBuff[2] = '0';
  bloodPressCorrectedBuf[0] = bloodPressCorrectedBuf[1] = bloodPressCorrectedBuf[2] = '0';
  bloodPressCorrectedBuf[8 * 3 + 0] =  bloodPressCorrectedBuf[8 * 3 + 1] = bloodPressCorrectedBuf[8 * 3 + 2] = '0';
  pulseRateCorrectedBuf[0] = pulseRateCorrectedBuf[1] = pulseRateCorrectedBuf[2] = '0';

  batteryState = 200;

  measurementSelection = NONE;
  alarmAcknowledge = TRUE;

  measureData.temperatureRawBuf = temperatureRawBuf;
  measureData.bloodPressRawBuf = bloodPressRawBuf;
  measureData.pulseRateRawBuf = pulseRateRawBuf;
  measureData.measurementSelection = &measurementSelection;

  computeData.temperatureRawBuf = temperatureRawBuf;
  computeData.bloodPressRawBuf = bloodPressRawBuf;
  computeData.pulseRateRawBuf = pulseRateRawBuf;
  computeData.measurementSelection = &measurementSelection;

  computeData.temperatureRawBuf = temperatureRawBuf;
  computeData.bloodPressRawBuf = bloodPressRawBuf;
  computeData.pulseRateRawBuf = pulseRateRawBuf;
  computeData.tempCorrectedBuff = tempCorrectedBuff;
  computeData.bloodPressCorrectedBuf = bloodPressCorrectedBuf;
  computeData.pulseRateCorrectedBuf = pulseRateCorrectedBuf;
  computeData.measurementSelection = &measurementSelection;

  displayData.tempCorrectedBuff = tempCorrectedBuff;
  displayData.bloodPressCorrectedBuf = bloodPressCorrectedBuf;
  displayData.pulseRateCorrectedBuf = pulseRateCorrectedBuf;
  displayData.batteryState = &batteryState;

  warningAlarmData.temperatureRawBuf = temperatureRawBuf;
  warningAlarmData.bloodPressRawBuf = bloodPressRawBuf;
  warningAlarmData.pulseRateRawBuf = pulseRateRawBuf;
  warningAlarmData.batteryState = &batteryState;
  warningAlarmData.alarmAcknowledge = &alarmAcknowledge;

  statusData.batteryState = &batteryState;

  keypadData.measurementSelection = &measurementSelection;
  keypadData.alarmAcknowledge = &alarmAcknowledge;

  taskList[0].taskDataPtr = &measureData;
  taskList[1].taskDataPtr = &computeData;
  taskList[2].taskDataPtr = &displayData;
  taskList[3].taskDataPtr = &warningAlarmData;
  taskList[4].taskDataPtr = &statusData;
  taskList[5].taskDataPtr = &keypadData;

  taskList[0].myTask = &Measure;
  taskList[1].myTask = &Compute;
  taskList[2].myTask = &Display;
  taskList[3].myTask = &WarningAlarm;
  taskList[4].myTask = &Status;
  taskList[5].myTask = &Keypad;

  for (int i = 0; i < 6; i++) {
    taskList[i].next = NULL;
    taskList[i].prev = NULL;
  }

  insert(&taskList[2]);
  insert(&taskList[3]);
  //insert(&taskList[5]);

  tft.fillScreen(BLACK);

  // create buttons
  //buttons[0].initButton(&tft, 100, 200, 50, 20, ILI9341_WHITE, buttoncolors[0], ILI9341_WHITE, buttonlabels[0], BUTTON_TEXTSIZE);
  //buttons[0].drawButton();
}


void loop() {
  // put your main code here, to run repeatedly:
  Schedule(taskList);
}

