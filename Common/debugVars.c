#include "debugVars.h"

volatile uint32_t debugQueueEventsSent = 0;
volatile uint32_t debugQueueEventsReceived = 0;
volatile GateEventType debugLastEventSent = EVENT_NONE;
volatile GateEventType debugLastEventReceived = EVENT_NONE;

volatile uint32_t debugMutexTakeCount = 0;
volatile uint32_t debugMutexGiveCount = 0;
volatile GateState debugLastStateWritten = GATE_IDLE_CLOSED;
volatile GateState debugLastStateRead = GATE_IDLE_CLOSED;

volatile uint32_t debugOpenLimitSemaphoreCount = 0;
volatile uint32_t debugClosedLimitSemaphoreCount = 0;
volatile uint32_t debugObstacleSemaphoreCount = 0;