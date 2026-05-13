#include "statusTask.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "appConfig.h"
#include "types.h"
#include "rtosObjects.h"

static const char *statusTaskStateToString(GateState state)
{
    switch (state)
    {
        case GATE_IDLE_CLOSED:
            return "IDLE_CLOSED";

        case GATE_IDLE_OPEN:
            return "IDLE_OPEN";

        case GATE_OPENING:
            return "OPENING";

        case GATE_CLOSING:
            return "CLOSING";

        case GATE_STOPPED_MIDWAY:
            return "STOPPED_MIDWAY";

        case GATE_REVERSING:
            return "REVERSING";

        default:
            return "UNKNOWN";
    }
}

static const char *statusTaskModeToString(ControlMode mode)
{
    switch (mode)
    {
        case CONTROL_MODE_NONE:
            return "NONE";

        case CONTROL_MODE_MANUAL:
            return "MANUAL";

        case CONTROL_MODE_AUTO:
            return "AUTO";

        default:
            return "UNKNOWN";
    }
}

static const char *statusTaskCommandToString(GateCommand command)
{
    switch (command)
    {
        case COMMAND_NONE:
            return "NONE";

        case COMMAND_OPEN:
            return "OPEN";

        case COMMAND_CLOSE:
            return "CLOSE";

        case COMMAND_STOP:
            return "STOP";

        default:
            return "UNKNOWN";
    }
}

void statusTask(void *pvParameters)
{
    GateState state;
    GateState previousState = GATE_IDLE_CLOSED;

    ControlMode mode;
    ControlMode previousMode = CONTROL_MODE_NONE;

    GateCommand command;
    GateCommand previousCommand = COMMAND_NONE;

    (void)pvParameters;

    printf("Status Task Started\r\n");

    while (1)
    {
        state = rtosObjectsGetGateState();
        mode = rtosObjectsGetControlMode();
        command = rtosObjectsGetActiveCommand();

        if (state != previousState || mode != previousMode || command != previousCommand)
        {
            printf("State: %s | Mode: %s | Command: %s\r\n",
                   statusTaskStateToString(state),
                   statusTaskModeToString(mode),
                   statusTaskCommandToString(command));

            previousState = state;
            previousMode = mode;
            previousCommand = command;
        }

        vTaskDelay(pdMS_TO_TICKS(STATUS_TASK_PERIOD_MS));
    }
}