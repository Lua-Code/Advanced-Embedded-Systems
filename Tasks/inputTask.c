#include "inputTask.h"

#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "appConfig.h"
#include "buttonDriver.h"
#include "rtosObjects.h"
#include "events.h"
#include "types.h"

#define BTN_DRIVER_OPEN_INDEX       0
#define BTN_DRIVER_CLOSE_INDEX      1
#define BTN_SECURITY_OPEN_INDEX     2
#define BTN_SECURITY_CLOSE_INDEX    3
#define BTN_OPEN_LIMIT_INDEX        4
#define BTN_CLOSED_LIMIT_INDEX      5
#define BTN_OBSTACLE_INDEX          6

#define BUTTON_COUNT                7

static void inputTaskSendEvent(GateEventType eventType, EventSource source)
{
    GateEvent event;

    event.type = eventType;
    event.source = source;
    event.timestampMs = xTaskGetTickCount() * portTICK_PERIOD_MS;

    xQueueSend(gateEventQueue, &event, pdMS_TO_TICKS(10));
}

void inputTask(void *pvParameters)
{
    uint32_t cur[BUTTON_COUNT] = {0};
    uint32_t prev[BUTTON_COUNT] = {0};

    GateEventType pressEvents[BUTTON_COUNT] =
    {
        EVENT_DRIVER_OPEN_PRESSED,
        EVENT_DRIVER_CLOSE_PRESSED,
        EVENT_SECURITY_OPEN_PRESSED,
        EVENT_SECURITY_CLOSE_PRESSED,
        EVENT_NONE,
        EVENT_NONE,
        EVENT_NONE
    };

    GateEventType releaseEvents[BUTTON_COUNT] =
    {
        EVENT_DRIVER_OPEN_RELEASED,
        EVENT_DRIVER_CLOSE_RELEASED,
        EVENT_SECURITY_OPEN_RELEASED,
        EVENT_SECURITY_CLOSE_RELEASED,
        EVENT_NONE,
        EVENT_NONE,
        EVENT_NONE
    };

    EventSource sources[BUTTON_COUNT] =
    {
        SOURCE_DRIVER,
        SOURCE_DRIVER,
        SOURCE_SECURITY,
        SOURCE_SECURITY,
        SOURCE_LIMIT,
        SOURCE_LIMIT,
        SOURCE_SAFETY
    };

    (void)pvParameters;

    while (1)
    {
        cur[BTN_DRIVER_OPEN_INDEX] = buttonDriverIsDriverOpenPressed();
        cur[BTN_DRIVER_CLOSE_INDEX] = buttonDriverIsDriverClosePressed();
        cur[BTN_SECURITY_OPEN_INDEX] = buttonDriverIsSecurityOpenPressed();
        cur[BTN_SECURITY_CLOSE_INDEX] = buttonDriverIsSecurityClosePressed();
        cur[BTN_OPEN_LIMIT_INDEX] = buttonDriverIsOpenLimitPressed();
        cur[BTN_CLOSED_LIMIT_INDEX] = buttonDriverIsClosedLimitPressed();
        cur[BTN_OBSTACLE_INDEX] = buttonDriverIsObstaclePressed();

        for (uint32_t i = 0; i < BUTTON_COUNT; i++)
        {
            if (cur[i] && !prev[i])
            {
                if (i == BTN_OPEN_LIMIT_INDEX)
                {
                    xSemaphoreGive(openLimitSemaphore);
                }
                else if (i == BTN_CLOSED_LIMIT_INDEX)
                {
                    xSemaphoreGive(closedLimitSemaphore);
                }
                else if (i == BTN_OBSTACLE_INDEX)
                {
                    xSemaphoreGive(obstacleSemaphore);
                }
                else
                {
                    inputTaskSendEvent(pressEvents[i], sources[i]);
                }
            }
            else if (!cur[i] && prev[i])
            {
                if (releaseEvents[i] != EVENT_NONE)
                {
                    inputTaskSendEvent(releaseEvents[i], sources[i]);
                }
            }

            prev[i] = cur[i];
        }

        vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE_MS));
    }
}