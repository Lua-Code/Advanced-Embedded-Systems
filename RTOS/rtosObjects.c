#include "rtosObjects.h"
#include "appConfig.h"

QueueHandle_t gateEventQueue = NULL;

SemaphoreHandle_t openLimitSemaphore = NULL;
SemaphoreHandle_t closedLimitSemaphore = NULL;
SemaphoreHandle_t obstacleSemaphore = NULL;

SemaphoreHandle_t gateStateMutex = NULL;

static GateState currentGateState = GATE_IDLE_CLOSED;
static ControlMode currentControlMode = CONTROL_MODE_NONE;
static GateCommand activeCommand = COMMAND_NONE;

void rtosObjectsInit(void)
{
    gateEventQueue = xQueueCreate(GATE_EVENT_QUEUE_LENGTH, sizeof(GateEvent));

    openLimitSemaphore = xSemaphoreCreateBinary();
    closedLimitSemaphore = xSemaphoreCreateBinary();
    obstacleSemaphore = xSemaphoreCreateBinary();

    gateStateMutex = xSemaphoreCreateMutex();
}

GateState rtosObjectsGetGateState(void)
{
    GateState state;

    xSemaphoreTake(gateStateMutex, portMAX_DELAY);
    state = currentGateState;
    xSemaphoreGive(gateStateMutex);

    return state;
}

void rtosObjectsSetGateState(GateState newState)
{
    xSemaphoreTake(gateStateMutex, portMAX_DELAY);
    currentGateState = newState;
    xSemaphoreGive(gateStateMutex);
}

ControlMode rtosObjectsGetControlMode(void)
{
    ControlMode mode;

    xSemaphoreTake(gateStateMutex, portMAX_DELAY);
    mode = currentControlMode;
    xSemaphoreGive(gateStateMutex);

    return mode;
}

void rtosObjectsSetControlMode(ControlMode newMode)
{
    xSemaphoreTake(gateStateMutex, portMAX_DELAY);
    currentControlMode = newMode;
    xSemaphoreGive(gateStateMutex);
}

GateCommand rtosObjectsGetActiveCommand(void)
{
    GateCommand command;

    xSemaphoreTake(gateStateMutex, portMAX_DELAY);
    command = activeCommand;
    xSemaphoreGive(gateStateMutex);

    return command;
}

void rtosObjectsSetActiveCommand(GateCommand newCommand)
{
    xSemaphoreTake(gateStateMutex, portMAX_DELAY);
    activeCommand = newCommand;
    xSemaphoreGive(gateStateMutex);
}