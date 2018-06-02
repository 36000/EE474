#ifndef _DATA_STRUCTS_H_
#define _DATA_STRUCTS_H_

#include "Boolean.h"

void Communicate(void* data); // for measure

extern unsigned long startingTime;
extern unsigned long systemTimeBase;

extern Bool bpHigh1;
extern Bool bpHigh2;
extern Bool tempHigh;
extern Bool pulseLow;
extern Bool respLow;

extern Bool measureFlag;
extern Bool computeFlag;
extern Bool displayFlag;
extern Bool warningAlarmFlag;
extern Bool statusFlag;
extern Bool keypadFlag;


extern Bool batteryLow;
extern Bool batteryDead;

extern unsigned int debounce;
extern unsigned char cuffInflation;

extern unsigned char bpOutOfRange;
extern unsigned char tempOutOfRange;
extern unsigned char pulseOutOfRange;
extern unsigned char respOutOfRange;

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

extern unsigned int tRawId, bp1RawId, bp2RawId, prRawId, rrRawId, tCorrId, bp1CorrId, bp2CorrId, prCorrId, rrCorrId;

typedef struct {
  unsigned int* temperatureRawBuf;
  unsigned int* bloodPressRawBuf;
  unsigned int* pulseRateRawBuf;
  unsigned int* respRateRawBuf;
  dt* measurementSelection;
} MeasureData;

typedef MeasureData CommunicateData; // they are the same

//extern dt measurementSelection; // for scheduler, REMOVE LATER
//extern dt alarmAcknowledge; // for scheduler, REMOVE LATER

extern dt menuMeas;
extern menu Menu;


typedef struct {
  unsigned int* temperatureRawBuf;
  unsigned int* bloodPressRawBuf;
  unsigned int* pulseRateRawBuf;
  unsigned int* respRateRawBuf;
  unsigned char* tempCorrectedBuff;
  unsigned char* bloodPressCorrectedBuf;
  unsigned char* pulseRateCorrectedBuf;
  unsigned char* respRateCorrectedBuf;
  dt* measurementSelection;
} ComputeData;

//extern ComputeData computeData; // delete this later, USED FOR RANDOMLY SELECTING MEASUREMENT TO UPDATE

typedef struct {
  unsigned char* tempCorrectedBuff;
  unsigned char* bloodPressCorrectedBuf;
  unsigned char* pulseRateCorrectedBuf;
  unsigned char* respRateCorrectedBuf;
  unsigned short* batteryState;
} DisplayData;

typedef struct {
  unsigned int* temperatureRawBuf;
  unsigned int* bloodPressRawBuf;
  unsigned int* pulseRateRawBuf;
  unsigned int* respRateRawBuf;
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

