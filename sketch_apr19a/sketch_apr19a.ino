#include "DataStructs.h"
#include "Tasks.h"

#include "Elegoo.h"

#include <arduino.h>
#include <stdio.h>
#include <stdlib.h>

// Touch Screen and Buttons initialization
Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
/*
Elegoo_GFX_Button buttons[5];
char buttonlabels[5][MAX_STRING_SIZE] = {"Select Data", "Warnings O/F", "Temp", "Blood Pres.", "Pulse Rate" };
uint16_t buttoncolors[5] = {ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE};                    
*/

Elegoo_GFX_Button buttons[15];
/* create 15 buttons, in classic candybar phone style */
char buttonlabels[5][5] = {"^", "v", "<", ">", "SEL"};
                             
// warning / alarms
Bool bpHigh1;
Bool bpHigh2;
Bool tempHigh;
Bool pulseLow;
Bool respLow;
Bool batteryLow;
Bool batteryDead;

TCB_ll ll;

// which menu are we in?
dt menuMeas;
menu Menu;

unsigned char bpOutOfRange;
unsigned char tempOutOfRange;
unsigned char pulseOutOfRange;
unsigned char respOutOfRange;

unsigned long systemTimeBase;
unsigned long startingTime;

static TCB taskList[6];

unsigned int tRawId, bp1RawId, bp2RawId, prRawId, rrRawId, tCorrId, bp1CorrId, bp2CorrId, prCorrId, rrCorrId;

static unsigned int temperatureRawBuf[8];
static unsigned int bloodPressRawBuf[16];
static unsigned int pulseRateRawBuf[8];
static unsigned int respRateRawBuf[8];

static unsigned char tempCorrectedBuff[8 * 3];
static unsigned char bloodPressCorrectedBuf[16 * 3];
static unsigned char pulseRateCorrectedBuf[8 * 3];
static unsigned char respRateCorrectedBuf[8 * 3];

static unsigned short batteryState;

static dt measurementSelection;
static dt alarmAcknowledge;

static MeasureData measureData;
static CommunicateData communicateData;
static ComputeData computeData;
static DisplayData displayData;
static WarningAlarmData warningAlarmData;
static StatusData statusData;
static KeypadData keypadData;

void ElegooSetup();

void setup() {
  ElegooSetup();

  menuMeas = TEMP;
  Menu = MEAS;
  
  ll.head = NULL;
  ll.tail = NULL;

  startingTime = millis();
  systemTimeBase = 0;

  bpHigh1 = FALSE;
  bpHigh2 = FALSE;
  tempHigh = FALSE;
  pulseLow = FALSE;
  respLow = FALSE;
  batteryLow = FALSE;
  batteryDead = FALSE;

  bpOutOfRange = 0;
  tempOutOfRange = 0;
  pulseOutOfRange = 0;
  respOutOfRange = 0;

  tRawId = bp1RawId = bp2RawId = prRawId = rrRawId = tCorrId = bp1CorrId = bp2CorrId = prCorrId = rrCorrId = 0;

  temperatureRawBuf[0] = 75;
  bloodPressRawBuf[0] = 80;
  bloodPressRawBuf[8] = 80;
  pulseRateRawBuf[0] = 50;
  respRateRawBuf[0] = 50;

  tempCorrectedBuff[0] = tempCorrectedBuff[1] = tempCorrectedBuff[2] = '0';
  bloodPressCorrectedBuf[0] = bloodPressCorrectedBuf[1] = bloodPressCorrectedBuf[2] = '0';
  bloodPressCorrectedBuf[8 * 3 + 0] =  bloodPressCorrectedBuf[8 * 3 + 1] = bloodPressCorrectedBuf[8 * 3 + 2] = '0';
  pulseRateCorrectedBuf[0] = pulseRateCorrectedBuf[1] = pulseRateCorrectedBuf[2] = '0';
  respRateCorrectedBuf[0] = respRateCorrectedBuf[1] = respRateCorrectedBuf[2] = '0';

  batteryState = 200;

  measurementSelection = NONE;
  alarmAcknowledge = NONE;

  measureData.temperatureRawBuf = temperatureRawBuf;
  measureData.bloodPressRawBuf = bloodPressRawBuf;
  measureData.pulseRateRawBuf = pulseRateRawBuf;
  measureData.respRateRawBuf = respRateRawBuf;
  measureData.measurementSelection = &measurementSelection;

  computeData.temperatureRawBuf = temperatureRawBuf;
  computeData.bloodPressRawBuf = bloodPressRawBuf;
  computeData.pulseRateRawBuf = pulseRateRawBuf;
  computeData.respRateRawBuf = respRateRawBuf;
  computeData.measurementSelection = &measurementSelection;
  computeData.tempCorrectedBuff = tempCorrectedBuff;
  computeData.bloodPressCorrectedBuf = bloodPressCorrectedBuf;
  computeData.pulseRateCorrectedBuf = pulseRateCorrectedBuf;
  computeData.respRateCorrectedBuf = respRateCorrectedBuf;

  displayData.tempCorrectedBuff = tempCorrectedBuff;
  displayData.bloodPressCorrectedBuf = bloodPressCorrectedBuf;
  displayData.pulseRateCorrectedBuf = pulseRateCorrectedBuf;
  displayData.respRateCorrectedBuf = respRateCorrectedBuf;
  displayData.batteryState = &batteryState;

  warningAlarmData.temperatureRawBuf = temperatureRawBuf;
  warningAlarmData.bloodPressRawBuf = bloodPressRawBuf;
  warningAlarmData.pulseRateRawBuf = pulseRateRawBuf;
  warningAlarmData.respRateRawBuf = respRateRawBuf;
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
  
  insert(&taskList[5]);
  insert(&taskList[2]);
  insert(&taskList[3]);
  insert(&taskList[0]);
  insert(&taskList[1]);
  insert(&taskList[4]);
  
  buttons[0].initButton(&tft, 160, 80, 40, 25, ILI9341_WHITE, ILI9341_BLUE, ILI9341_WHITE, buttonlabels[0], BUTTON_TEXTSIZE);
  buttons[1].initButton(&tft, 160, 146, 40, 25, ILI9341_WHITE, ILI9341_BLUE, ILI9341_WHITE, buttonlabels[1], BUTTON_TEXTSIZE);
  buttons[2].initButton(&tft, 90, 113, 40, 25, ILI9341_WHITE, ILI9341_BLUE, ILI9341_WHITE, buttonlabels[2], BUTTON_TEXTSIZE);
  buttons[3].initButton(&tft, 230, 113, 40, 25, ILI9341_WHITE, ILI9341_BLUE, ILI9341_WHITE, buttonlabels[3], BUTTON_TEXTSIZE);
  buttons[4].initButton(&tft, 160, 113, 80, 25, ILI9341_WHITE, ILI9341_BLUE, ILI9341_WHITE, buttonlabels[4], BUTTON_TEXTSIZE);
  for (int i = 0; i < 5; i++){
    buttons[i].drawButton();
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  Schedule(taskList);
}

