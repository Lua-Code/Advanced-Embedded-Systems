#include "gateControlTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "events.h"
#include "types.h"
#include "appConfig.h"

#include "rtosObjects.h"
#include "gateStateMachine.h"

void gateControlTask(void *pvParameters)
{
    GateEvent event;
    GateState newState;

    (void)pvParameters;

    while (1)
    {
        if (xQueueReceive(gateEventQueue, &event, portMAX_DELAY) == pdTRUE)
        {
            newState = gateStateMachineHandleEvent(event);

            rtosObjectsSetGateState(newState);
            rtosObjectsSetControlMode(gateStateMachineGetControlMode());
            rtosObjectsSetActiveCommand(gateStateMachineGetActiveCommand());

            if (newState == GATE_REVERSING)
            {
                vTaskDelay(pdMS_TO_TICKS(OBSTACLE_REVERSE_TIME_MS));

                gateStateMachineFinishReversing();

                rtosObjectsSetGateState(gateStateMachineGetState());
                rtosObjectsSetControlMode(gateStateMachineGetControlMode());
                rtosObjectsSetActiveCommand(gateStateMachineGetActiveCommand());
            }
        }
    }
}