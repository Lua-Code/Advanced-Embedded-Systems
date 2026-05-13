#ifndef RTOS_OBJECTS_H
#define RTOS_OBJECTS_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "types.h"
#include "events.h"

extern QueueHandle_t gateEventQueue;

extern SemaphoreHandle_t openLimitSemaphore;
extern SemaphoreHandle_t closedLimitSemaphore;
extern SemaphoreHandle_t obstacleSemaphore;

extern SemaphoreHandle_t gateStateMutex;

void rtosObjectsInit(void);

GateState rtosObjectsGetGateState(void);
void rtosObjectsSetGateState(GateState newState);

ControlMode rtosObjectsGetControlMode(void);
void rtosObjectsSetControlMode(ControlMode newMode);

GateCommand rtosObjectsGetActiveCommand(void);
void rtosObjectsSetActiveCommand(GateCommand newCommand);

#endif