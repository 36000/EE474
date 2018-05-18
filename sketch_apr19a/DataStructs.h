#ifndef _DATA_STRUCTS_H_
#define _DATA_STRUCTS_H_

#include "Boolean.h"

extern unsigned long startingTime;
extern unsigned long systemTimeBase;

extern Bool bpHigh;
extern Bool tempHigh;
extern Bool pulseLow;

extern Bool batteryLow;
extern Bool batteryDead;

extern unsigned char bpOutOfRange;
extern unsigned char tempOutOfRange;
extern unsigned char pulseOutOfRange;

extern Bool alarmAcknowledge;

typedef struct TCB TCB;
typedef struct TCB_ll TCB_ll;
extern TCB_ll ll;

struct TCB_ll {
  TCB* head;
  TCB* tail;
};

struct TCB {
  void (*myTask)(void*);
  void* taskDataPtr;
  TCB* prev;
  TCB* next;
};


typedef enum {NONE, TEMP, BLOOD1, BLOOD2, PULSE, RESP} dt;
typedef enum {MEAS, ANUN} menu;

extern unsigned int tRawId, bp1RawId, bp2RawId, prRawId, tCorrId, bp1CorrId, bp2CorrId, prCorrId;

typedef struct {
  unsigned int* temperatureRawBuf;
  unsigned int* bloodPressRawBuf;
  unsigned int* pulseRateRawBuf;
  dt* measurementSelection;
} MeasureData;

typedef MeasureData CommunicateData; // they are the same

extern dt measurementSelection; // for scheduler, REMOVE LATER
extern dt menuMeas;
extern menu Menu;


typedef struct {
  unsigned int* temperatureRawBuf;
  unsigned int* bloodPressRawBuf;
  unsigned int* pulseRateRawBuf;
  unsigned char* tempCorrectedBuff;
  unsigned char* bloodPressCorrectedBuf;
  unsigned char* pulseRateCorrectedBuf;
  dt* measurementSelection;
} ComputeData;

extern ComputeData computeData; // delete this later, USED FOR RANDOMLY SELECTING MEASUREMENT TO UPDATE

typedef struct {
  unsigned char* tempCorrectedBuff;
  unsigned char* bloodPressCorrectedBuf;
  unsigned char* pulseRateCorrectedBuf;
  unsigned short* batteryState;
} DisplayData;

typedef struct {
  unsigned int* temperatureRawBuf;
  unsigned int* bloodPressRawBuf;
  unsigned int* pulseRateRawBuf;
  unsigned short* batteryState;
  dt* alarmAcknowledge;
} WarningAlarmData;

typedef struct {
  unsigned short* batteryState;
} StatusData;

typedef struct {
  dt* measurementSelection;
  dt* alarmAcknowledge;
} KeypadData;

#endif // _DATA_STRUCTS_H_

