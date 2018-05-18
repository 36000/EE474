#include "DataStructs.h"

void WarningAlarm (void* data) {
  WarningAlarmData* warningAlarmData = (WarningAlarmData*) data;

  unsigned int temp = warningAlarmData->temperatureRawBuf[tRawId];
  unsigned int sysPress = warningAlarmData->bloodPressRawBuf[bp1RawId];
  unsigned int diasPress = warningAlarmData->bloodPressRawBuf[bp2RawId + 8];
  unsigned int pulse = warningAlarmData->pulseRateRawBuf[prRawId];
  unsigned int resp = warningAlarmData->respRateRawBuf[rrRawId];


  temp = 50 + 7.5 * temp;
  sysPress = 9 + 2 * sysPress;
  diasPress = 6 + 1.5 * diasPress;
  pulse = 8 + 3 * pulse;
  resp = 0;

  // warnings
  if (temp <= 361*1.05 || temp >= 378*1.05) // check if out of warning range
    tempHigh = TRUE;
  else
    tempHigh = FALSE;

  if (sysPress <= 120*1.2 || sysPress >= 130*1.2) // check if out of warning range
    bpHigh1 = TRUE;
  else
    bpHigh1 = FALSE;

  if (sysPress <= 120*1.2 || sysPress >= 130*1.2) // check if out of warning range
    bpHigh2 = TRUE;
  else
    bpHigh2 = FALSE;

  if (pulse <= 60*1.05 || pulse >= 100*1.05) // check if out of warning range
    pulseLow = TRUE;
  else
    pulseLow = FALSE;

  if (resp <= 12*1.05 || resp >= 25*1.05) // check if out of warning range
    respLow = TRUE;
  else
    respLow = FALSE;

  // alarms

  if (sysPress <= 120*1.2 || sysPress >= 130*1.2) { //check if out of alarm range
    if (bpOutOfRange == 0)
      bpOutOfRange = 1;

    if (bpOutOfRange != 1)
      bpOutOfRange -= 2; // alarm timer, eventuallly turns back on

    if (*warningAlarmData->alarmAcknowledge == BLOOD1 || *warningAlarmData->alarmAcknowledge == BLOOD2) {
      bpOutOfRange = 89; // turn alarm off, set high number for timer which steadily decrements
      *warningAlarmData->alarmAcknowledge = NONE;
    }
  } else {
    bpOutOfRange = 0; // indicate alarm has not been thrown yet
  }

  if (temp <= 361*1.15 || temp >= 378*1.15) { //check if out of alarm range
    if (tempOutOfRange == 0)
      tempOutOfRange = 1;

    if (tempOutOfRange != 1)
      tempOutOfRange -= 2; // alarm timer, eventuallly turns back on

    if (*warningAlarmData->alarmAcknowledge == TEMP) {
      tempOutOfRange = 89; // turn alarm off, set high number for timer which steadily decrements
      *warningAlarmData->alarmAcknowledge = NONE;
    }
  } else {
    tempOutOfRange = 0; // indicate alarm has not been thrown yet
  }

  if (pulse <= 60*1.15 || pulse >= 100*1.15) { //check if out of alarm range
    if (pulseOutOfRange == 0)
      pulseOutOfRange = 1;

    if (pulseOutOfRange != 1)
      pulseOutOfRange -= 2; // alarm timer, eventuallly turns back on

    if (*warningAlarmData->alarmAcknowledge == PULSE) {
      pulseOutOfRange = 89; // turn alarm off, set high number for timer which steadily decrements
      *warningAlarmData->alarmAcknowledge = NONE;
    }
  } else {
    pulseOutOfRange = 0; // indicate alarm has not been thrown yet
  }

  if (resp <= 12*1.15 || resp >= 25*1.15) { //check if out of alarm range
    if (respOutOfRange == 0)
      respOutOfRange = 1;

    if (respOutOfRange != 1)
      respOutOfRange -= 2; // alarm timer, eventuallly turns back on

    if (*warningAlarmData->alarmAcknowledge == PULSE) {
      respOutOfRange = 89; // turn alarm off, set high number for timer which steadily decrements
      *warningAlarmData->alarmAcknowledge = NONE;
    }
  } else {
    respOutOfRange = 0; // indicate alarm has not been thrown yet
  }

  // battery stuff

  if (*warningAlarmData->batteryState < 40)
    batteryLow = TRUE;

  if (!batteryDead && *warningAlarmData->batteryState > 200)
    batteryDead = TRUE;
}
