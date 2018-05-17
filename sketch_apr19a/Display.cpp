#include "DataStructs.h"
#include "Elegoo.h"
#include "Boolean.h"
// Note: Lines that are 20 characters wide will automatically overflow to next line, so sometimes no \n character is needed.

void Display (void* data) {
  Bool anyAlarm = FALSE;
  DisplayData* displayData = (DisplayData*) data;

  // Clearing regions of the screen that get updated when Display is called
  // each line is 16 pixels tall, 20 characters wide
  // each character is 12 pixels wide
  //tft.fillRect(x,y,width,height,color);

  tft.setCursor(0, 0);
  tft.setTextColor(CYAN);
  tft.setTextSize(2);

  // When battery dies only print "Battery Dead"
  if (batteryDead) {
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.print("Battery Dead");
    return;
  }

  // Print header and title
  tft.print(" Pocket Doctor (TM)\n");
  tft.print("--------------------\n");


  tft.fillRect(11 * 12, 3 * 16, 4 * 12, 1 * 16, BLACK);
  tft.setTextColor(WHITE);
  tft.print("Body Temp: ");
  // Set color of text for body temp warning, similar use in all other sections
  if (tempHigh) {
    tft.setTextColor(RED);
  } else {
    tft.setTextColor(GREEN);
  }
  // Print body temperature data
  for (int i = 0; i < 3; i++) {
    if (i == 2) tft.print('.');
    tft.print((char) displayData->tempCorrectedBuff[tCorrId * 3 + i]);
    Serial.print(displayData->tempCorrectedBuff[tCorrId * 3]);
  }
  tft.setTextColor(WHITE); tft.print(" C\n");


  tft.fillRect(11 * 12, 4 * 16, 3 * 12, 1 * 16, BLACK);
  tft.print("Syst Pres: ");
  // Print blood pressure data
  if (bpHigh) {
    tft.setTextColor(RED);
  } else {
    tft.setTextColor(GREEN);
  }

  for (int i = 0; i < 3; i++)
    tft.print((char) displayData->bloodPressCorrectedBuf[bp1CorrId * 3 + i]);
  tft.setTextColor(WHITE); tft.print(" mm Hg");


  tft.fillRect(11 * 12, 5 * 16, 3 * 12, 1 * 16, BLACK);
  tft.print("Dias Pres: ");
  if (bpHigh) {
    tft.setTextColor(RED);
  } else {
    tft.setTextColor(GREEN);
  }
  for (int i = 0; i < 3; i++)
    tft.print((char) displayData->bloodPressCorrectedBuf[bp2CorrId * 3 + i + 8 * 3]);
  tft.setTextColor(WHITE); tft.print(" mm Hg");


  tft.fillRect(11 * 12, 6 * 16, 3 * 12, 1 * 16, BLACK);
  tft.print("Pulse Rate:");
  // Print pulse rate data
  if (pulseLow) {
    tft.setTextColor(RED);
  } else {
    tft.setTextColor(GREEN);
  }
  for (int i = 0; i < 3; i++)
    tft.print((char) displayData->pulseRateCorrectedBuf[prCorrId * 3 + i]);
  tft.setTextColor(WHITE); tft.print(" BPM\n");


  tft.fillRect(11 * 12, 7 * 16, 3 * 12, 1 * 16, BLACK);
  tft.print("Battery:   ");
  // Print battery status
  if (batteryLow) {
    tft.setTextColor(RED);
  } else {
    tft.setTextColor(GREEN);
  }
  // Adding a leading zero when batteryState is only two digits
  if (*displayData->batteryState < 100)
    tft.print("0");
  tft.print(*displayData->batteryState);
  tft.setTextColor(WHITE); tft.print("/200\n");


  tft.fillRect(0 * 12, 9 * 16, 20 * 12, 3 * 16, BLACK);
  // Print alarms
  tft.print("Alarms:\n");
  tft.setTextColor(RED);
  if (tempOutOfRange != 0 && tempOutOfRange < 10) {
    tft.print("BODY TEMP ALARM\n");
    anyAlarm = TRUE;
  }
  if (bpOutOfRange != 0 && bpOutOfRange < 10) {
    tft.print("BLOOD PRESSURE ALARM");
    anyAlarm = TRUE;
  }

  if (pulseOutOfRange != 0 && pulseOutOfRange < 10) {
    tft.print("PULSE RATE ALARM\n");
    anyAlarm = TRUE;
  }

  if (anyAlarm == FALSE) {
    tft.setTextColor(GREEN);
    tft.print("none");
  }
  tft.fillScreen(BLACK);
}

