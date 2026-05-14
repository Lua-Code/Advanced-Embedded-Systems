#ifndef DEBUG_VARS_H
#define DEBUG_VARS_H

#include <stdint.h>
#include "types.h"
#include "events.h"

extern volatile uint32_t debugQueueEventsSent;
extern volatile uint32_t debugQueueEventsReceived;
extern volatile GateEventType debugLastEventSent;
extern volatile GateEventType debugLastEventReceived;

extern volatile uint32_t debugMutexTakeCount;
extern volatile uint32_t debugMutexGiveCount;
extern volatile GateState debugLastStateWritten;
extern volatile GateState debugLastStateRead;

extern volatile uint32_t debugOpenLimitSemaphoreCount;
extern volatile uint32_t debugClosedLimitSemaphoreCount;
extern volatile uint32_t debugObstacleSemaphoreCount;

#endif