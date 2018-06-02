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
              *keypadData->measurementSelection = menuMeas;
            } else {
              *keypadData->alarmAcknowledge = menuMeas;
            }
            break;
          default:
            break;
        }
        delay(500); // UI debouncing
    }
  }
}





