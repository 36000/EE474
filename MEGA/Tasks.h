
#ifndef _TASKS_H_
#define _TASKS_H_

void Measure (void* data);

void Compute (void* data);

void WarningAlarm (void* data);

void Status (void* data);

void Display (void* data);

void Keypad(void* data);

void Schedule (TCB* taskList);

#endif // _SCHEDULE_H_
