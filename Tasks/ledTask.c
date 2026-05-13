#include "ledTask.h"

#include "FreeRTOS.h"
#include "task.h"

#include "appConfig.h"
#include "types.h"

#include "rtosObjects.h"
#include "ledDriver.h"

void ledTask(void *pvParameters)
{
    GateState state;

    (void)pvParameters;

    while (1)
    {
        state = rtosObjectsGetGateState();

        switch (state)
        {
            case GATE_OPENING:
            case GATE_REVERSING:
                ledDriverGreenOn();
                ledDriverRedOff();
                break;

            case GATE_CLOSING:
                ledDriverGreenOff();
                ledDriverRedOn();
                break;

            case GATE_IDLE_OPEN:
            case GATE_IDLE_CLOSED:
            case GATE_STOPPED_MIDWAY:
            default:
                ledDriverAllOff();
                break;
        }

        vTaskDelay(pdMS_TO_TICKS(LED_TASK_PERIOD_MS));
    }
}