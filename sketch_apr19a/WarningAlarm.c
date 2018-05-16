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


  if (sysPress > 120 || diasPress > 80) {
    if (!bpOutOfRange)
      bpOutOfRange = 89; // signal for alarm
    else
      bpOutOfRange -= 2; // alarm timer, eventually shuts off when under 0
    bpHigh = TRUE;
  } else {
    bpHigh = FALSE;
    bpOutOfRange = 0; // indicate alarm has not been thrown yet
  }

  if (temp < 361 || temp > 378) {
    if (!tempOutOfRange)
      tempOutOfRange = 89; // signal for alarm
    else
      tempOutOfRange -= 2; // alarm timer, eventually shuts off when under 0
    tempHigh = TRUE;
  } else {
    tempHigh = FALSE;
    tempOutOfRange = 0; // indicate alarm has not been thrown yet
  }

  if (pulse < 60 || pulse > 100) {
    if (!pulseOutOfRange)
      pulseOutOfRange = 89; // signal for alarm
    else
      pulseOutOfRange -= 2; // alarm timer, eventually shuts off when under 0
    pulseLow = TRUE;
  } else {
    pulseLow = FALSE;
    pulseOutOfRange = 0; // indicate alarm has not been thrown yet
  }

  if (*warningAlarmData->batteryState < 40)
    batteryLow = TRUE;

  if (!batteryDead && *warningAlarmData->batteryState > 200)
    batteryDead = TRUE;
}
