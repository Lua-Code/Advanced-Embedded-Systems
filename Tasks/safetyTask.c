#include "types.h"
#include "TM4C123GH6PM.h"

#include <stdint.h>
#include <stdbool.h>

#include "sysctl.h"

#include "gpioDriver.h"
#include "ledDriver.h"
#include "buttonDriver.h"

#include "FreeRTOS.h"
#include "task.h"
#include "basic_io.h"
#include "TM4C123.h"
#include "core_cm4.h"
#include "semphr.h"
#include "gateStateMachine.h"
#include "rtosObjects.h"

static void safetyTaskSendEvent(GateEventType eventType,EventSource source)
{
	GateEvent event;
	
	event.type = eventType;
	event.source = source;
	event.timestampMs = xTaskGetTickCount() * portTICK_PERIOD_MS;
	
	xQueueSend(gateEventQueue,&event,pdMS_TO_TICKS(20));

}


void safetyTask(void *pvParameters) {
	while(1)
	{
		if(xSemaphoreTake(openLimitSemaphore,0) == pdTRUE)
		{
			safetyTaskSendEvent(EVENT_OPEN_LIMIT_PRESSED,SOURCE_LIMIT);
		}
		
		if(xSemaphoreTake(closedLimitSemaphore,0) == pdTRUE)
		{
			safetyTaskSendEvent(EVENT_CLOSED_LIMIT_PRESSED,SOURCE_LIMIT);
		}
			
		if(xSemaphoreTake(obstacleSemaphore,0) == pdTRUE)
		{
			if(rtosObjectsGetGateState() == GATE_CLOSING)
			{
				safetyTaskSendEvent(EVENT_OBSTACLE_DETECTED,SOURCE_SAFETY);
			}
		}
	}
}