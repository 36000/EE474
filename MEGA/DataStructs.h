#ifndef _DATA_STRUCTS_H_
#define _DATA_STRUCTS_H_

#include "Boolean.h"

#define taskNumber 11

void Communicate(void* data); // for measure

extern unsigned long startingTime;
extern unsigned long systemTimeBase;

extern int isCuffReady;

extern Bool bpHigh1;
extern Bool bpHigh2;
extern Bool tempHigh;
extern Bool pulseLow;
extern Bool respLow;

extern Bool taskFlags[taskNumber];

extern Bool batteryLow;
extern Bool batteryDead;

extern unsigned long debounce;
extern unsigned char cuffInflation;

extern unsigned char bpOutOfRange;
extern unsigned char tempOutOfRange;
extern unsigned char pulseOutOfRange;
extern unsigned char respOutOfRange;

typedef struct TCB TCB;
typedef struct TCB_ll TCB_ll;
extern TCB_ll llTCB;
extern TCB_ll llData;
extern TCB dataList[6];

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

void del(TCB* node, TCB_ll* ll);
void insert(TCB* node, TCB_ll* ll);
bool check(TCB* node, TCB_ll* ll);

typedef enum {NONE, TEMP, BLOOD1, BLOOD2, PULSE, RESP} dt;
typedef enum {NON, I, S, P, M, W} sig;
typedef enum {MEAS, ANUN} menu;

extern sig commSig;

extern unsigned int tRawId, bp1RawId, bp2RawId, prRawId, rrRawId, tCorrId, bp1CorrId, bp2CorrId, prCorrId, rrCorrId, EKGId;

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
  unsigned int* EKGFreqBuff;
  unsigned short* batteryState;
} DisplayData;

typedef DisplayData RemoteDispData; // they are the same

typedef struct {
  unsigned int* temperatureRawBuf;
  unsigned int* bloodPressRawBuf;
  unsigned int* pulseRateRawBuf;
  unsigned int* respRateRawBuf;
  unsigned int* EKGFreqBuff;
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

typedef KeypadData RemoteCommData;

typedef struct {
  unsigned int* EKGRawBuff;
  unsigned int* EKGFreqBuff;
} EKGData;

#endif // _DATA_STRUCTS_H_

