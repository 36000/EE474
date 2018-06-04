#include "DataStructs.h"
#include "Elegoo.h"
#include "Boolean.h"
// Note: Lines that are 20 characters wide will automatically overflow to next line, so sometimes no \n character is needed.

dt prevMenuMeas = TEMP;
menu prevMenu = MEAS;
Bool hasChanged = FALSE;
Bool prevOff = FALSE;

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
    tft.print("Screen Off \nDue To: \nBattery Dead or \nScreen Turned Off");
    prevOff = TRUE;
    return;
  }

  if (prevOff) {
    prevOff = FALSE;
    tft.fillScreen(BLACK);
    tft.setTextColor(CYAN);
  }

  // Print header and title
  tft.print(" Pocket Doctor (TM)\n");
  tft.print("--------------------\n");

  if (prevMenuMeas == menuMeas && prevMenu == Menu) {
    hasChanged = FALSE;
  } else {
    prevMenuMeas = menuMeas;
    prevMenu = Menu;
    hasChanged = TRUE;
  }

  // print header
  if (hasChanged)
    tft.fillRect(0 * 12, 2 * 16, 25 * 12, 1 * 16, BLACK);
  tft.setTextColor(YELLOW);
  if (Menu == ANUN) {
    tft.print("Annunciating ");
  } else {
    tft.print("SEL to measure ");
  }
  switch (menuMeas) { // TEMP, BLOOD1, BLOOD2, PULSE, RESP
    case TEMP:
      tft.print("Body Temp\n");
      break;
    case BLOOD1:
      tft.print("Sys Press\n");
      break;
    case BLOOD2:
      tft.print("Dias Press\n");
      break;
    case PULSE:
      tft.print("Pulse Rate\n");
      break;
    case RESP:
      tft.print("Resp Rate\n");
      break;
    default:
      tft.print("\n");
      break;
  }

  if (hasChanged)
    tft.fillRect(0 * 12, 3 * 16, 25 * 12, 1 * 16, BLACK);
  else
     tft.fillRect(11 * 12, 3 * 16, 5 * 12, 1 * 16, BLACK);
     
  if (Menu == ANUN) {
    const char* measType;
    const char* units;
    char values[4];
    Bool warning;

    switch (menuMeas) { // TEMP, BLOOD1, BLOOD2, PULSE, RESP
      case TEMP:
        measType = "Body Temp: ";
        units = " C\n";
        if (systemTimeBase % 4 < 2)
          warning = tempHigh;
        else
          warning = FALSE;
        values[0] = (char) displayData->tempCorrectedBuff[tCorrId * 3];
        values[1] = (char) displayData->tempCorrectedBuff[tCorrId * 3 + 1];
        values[2] = '.';
        values[3] = (char) displayData->tempCorrectedBuff[tCorrId * 3 + 2];
        break;
     case BLOOD1:
        measType = "Syst Pres: ";
        units = " mm Hg\n";
        if (systemTimeBase % 2 == 0)
          warning = bpHigh1;
        else
          warning = FALSE;
        for (int i = 0; i < 3; i++)
          values[i] = (char) displayData->bloodPressCorrectedBuf[bp1CorrId * 3 + i];
        values[3] = ' ';
        break;
     case BLOOD2:
        measType = "Dias Pres: ";
        units = " mm Hg\n";
        if (systemTimeBase % 2 == 0)
          warning = bpHigh2;
        else
          warning = FALSE;
        for (int i = 0; i < 3; i++)
          values[i] = (char) displayData->bloodPressCorrectedBuf[bp2CorrId * 3 + i + 8 * 3];
        values[3] = ' ';
        break;
     case PULSE:
        measType = "Pulse Rate:";
        units = " BPM\n";
        if (systemTimeBase % 8 < 4)
          warning = pulseLow;
        else
          warning = FALSE;
        for (int i = 0; i < 3; i++)
          values[i] = (char) displayData->pulseRateCorrectedBuf[prCorrId * 3 + i];
        values[3] = ' ';
        break;
     case RESP:
        measType = "Resp Rate: ";
        units = " RR\n";
        warning = respLow;
        for (int i = 0; i < 3; i++)
          values[i] = (char) displayData->respRateCorrectedBuf[rrCorrId * 3 + i];
        values[3] = ' ';
        break;
     default:
        measType = "ERROR";
        units = "ERROR";
        warning = TRUE;
        for (int i = 0; i < 4; i++)
          values[i] = ' ';
        break;
    }
    
    tft.setTextColor(WHITE);
    tft.print(measType);
    // Set warning
    if (warning) {
      tft.setTextColor(ORANGE);
    } else {
      tft.setTextColor(GREEN);
    }
    for (int i = 0; i < 4; i++)
      tft.print(values[i]);
    tft.setTextColor(WHITE); tft.print(units);
  }
  
  //warning and alarms
  tft.setCursor(0, 161-16);
  tft.fillRect(0 * 12, 10 * 16, 26 * 12, 2 * 16, BLACK);
  // Print alarms
  tft.setTextColor(WHITE); tft.print("Alarms: \n");
  tft.setTextColor(RED);
  if (tempOutOfRange == 1) {
    tft.print("TEMPERATURE, ");
    anyAlarm = TRUE;
  }
  if (bpOutOfRange == 1) {
    tft.print("PRESSURE, ");
    anyAlarm = TRUE;
  }

  if (pulseOutOfRange == 1) {
    tft.print("PULSE RATE, ");
    anyAlarm = TRUE;
  }

  if (anyAlarm == FALSE) {
    tft.setTextColor(GREEN);
    tft.print("none");
  } // try to keep warnings / alarms to two lines

  // battery
  tft.setCursor(0, 161+32);
  tft.fillRect(11 * 12, 12 * 16, 3 * 12, 1 * 16, BLACK);
  tft.setTextColor(WHITE); tft.print("Battery:   ");
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

  tft.fillRect(16 * 12, 13 * 16, 4 * 12, 1 * 16, BLACK);
  tft.print("Cuff Inflation: ");
  tft.print(cuffInflation);
  tft.print("0%");
}

