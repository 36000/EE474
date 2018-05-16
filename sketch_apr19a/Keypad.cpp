#include "DataStructs.h"
#include "Boolean.h"

void Keypad (void* data) {
  
  /*KeypadData* keypadData = (KeypadData*) data;
  
  for (int b=0; b<5; b++) {
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
          *keypadData->alarmAcknowledge = 1 - *keypadData->alarmAcknowledge;
        } 
        if (b <= 2 && *keypadData->dataMenu) {
          *keypadData->displayMenu = TRUE;
          *keypadData->dataMenu = FALSE;
          if (b == 2) *keypadData->measurementSelection = TEMP;
          if (b == 3) *keypadData->measurementSelection = BLOOD;
          if (b == 4) *keypadData->measurementSelection = PULSE;
        }
          
    }
  }*/
}



