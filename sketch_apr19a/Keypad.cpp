#include "DataStructs.h"
#include "Boolean.h"
#include "Elegoo.h"

void Keypad (void* data) {
  KeypadData* keypadData = (KeypadData*) data;

  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
   if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width (fits values of x and y to the screen)
    p.x = (tft.width() - map(p.x, TS_MINX, TS_MAXX, tft.width(), 0));
    p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
   }
   
  // go thru all the buttons, checking if they were pressed
  for (uint8_t b=0; b<5; b++) {
    if (buttons[b].contains(p.x, p.y)) {
      buttons[b].press(true);  // tell the button it is pressed
    } else {
      buttons[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // now we can ask the buttons if their state has changed
  for (uint8_t b=0; b<5; b++) {
    if (buttons[b].justReleased()) {
      // Serial.print("Released: "); Serial.println(b);
      buttons[b].drawButton();  // draw normal
    }
    
    if (buttons[b].justPressed()) {
        buttons[b].drawButton(true);  // draw invert!
        // if a numberpad button, append the relevant # to the textfield 
        int idt; // integer to represenet data type
        switch(b) { // up, down, left, right, select
          case 0:
            if (Menu == MEAS)
              Menu = ANUN;
            else
              Menu = MEAS;
            break;
          case 1:
            if (Menu == MEAS)
              Menu = ANUN;
            else
              Menu = MEAS;
            break;
          case 2:
            idt = (int) menuMeas;
            idt--;
            if (idt == 0)
              idt = 5;
            menuMeas = (dt) idt;
            break;
          case 3:
            idt = (int) menuMeas;
            idt++;
            if (idt == 6)
              idt = 1;
            menuMeas = (dt) idt;
            break;
          case 4:
            if (Menu == MEAS) {
              *data->measurementSelection = menuMeas;
            } else {
              *data->alarmAcknowledge = menuMeas;
            }
            break;
          default:
            break;
        }
        delay(100); // UI debouncing
    }
  }
  /*
   * assume there are only 5 buttons for this commented code
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    p.x = tft.width() - map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
  }
  
  for (int b=0; b<1; b++) {
    if (buttons[b].contains(p.x, p.y)) {
      buttons[b].press(true);
    } else {
      buttons[b].press(false);
    }
    delay(10);
    
    if (buttons[b].justReleased()) {
      // Serial.print("Released: "); Serial.println(b);
      buttons[b].drawButton();  // draw normal
    }
    
    if (buttons[b].justPressed()) {
        buttons[b].drawButton(true);  // draw invert!
        
        // data menu button
        if (b == 0) {
          *keypadData->dataMenu = TRUE;
          *keypadData->displayMenu = FALSE;
        }
        // toggle alarms button
        if (b == 1 && *keypadData->displayMenu) {
          *keypadData->alarmAcknowledge = (Bool)(1 - (int)*keypadData->alarmAcknowledge);
        } 
        if (b <= 2 && *keypadData->dataMenu) {
          *keypadData->displayMenu = TRUE;
          *keypadData->dataMenu = FALSE;
          if (b == 2) *keypadData->measurementSelection = TEMP;
          if (b == 3) *keypadData->measurementSelection = BLOOD;
          if (b == 4) *keypadData->measurementSelection = PULSE;
        }
        
    }
  }
  */
}

// Print something in the mini status bar with either flashstring
void status(const __FlashStringHelper *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, ILI9341_BLACK);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.print(msg);
}

// or charstring
void status(char *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, ILI9341_BLACK);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.print(msg);
}






