#include "DataStructs.h"

void WarningAlarm (void* data) {
  WarningAlarmData* warningAlarmData = (WarningAlarmData*) data;

  unsigned int temp = warningAlarmData->temperatureRawBuf[tRawId];
  unsigned int sysPress = warningAlarmData->bloodPressRawBuf[bp1RawId];
  unsigned int diasPress = warningAlarmData->bloodPressRawBuf[bp2RawId + 8];
  unsigned int pulse = warningAlarmData->pulseRateRawBuf[prRawId];

  temp = 50 + 7.5 * temp;
  sysPress = 9 + 2 * sysPress;
  diasPress = 6 + 1.5 * diasPress;
  pulse = 8 + 3 * pulse;

  // example for syspress
  if (sysPress >= 0) { //check if out of alarm range
    if (bpOutOfRange == 0)
      bpOutOfRange = 1;
      
    if (bpOutOfRange != 1)
      bpOutOfRange -= 2; // alarm timer, eventuallly turns back on
        
    if (*warningAlarmData->alarmAcknowledge == BLOOD1) {
        bpOutOfRange = 89; // turn alarm off, set high number for timer which steadily decrements
        *warningAlarmData->alarmAcknowledge = NONE;
    }
  } else {
    bpOutOfRange = 0; // indicate alarm has not been thrown yet
  }
  if (sysPress >= 0) // check if out of warning range
    bpHigh = TRUE;
  else
    bpHigh = FALSE;

  //end syspress example

  if (*warningAlarmData->batteryState < 40)
    batteryLow = TRUE;

  if (!batteryDead && *warningAlarmData->batteryState > 200)
    batteryDead = TRUE;
}
