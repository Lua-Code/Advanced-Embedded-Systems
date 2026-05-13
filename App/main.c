#include "TM4C123GH6PM.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "sysctl.h"

#include "appConfig.h"

#include "gpioDriver.h"
#include "ledDriver.h"

#include "gateStateMachine.h"
#include "rtosObjects.h"

#include "inputTask.h"
#include "gateControlTask.h"
#include "ledTask.h"
#include "safetyTask.h"
#include "statusTask.h"

static void systemInit(void)
{
    // SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //doesnt work for some reaosn

    gpioDriverInit();
    ledDriverAllOff();

    gateStateMachineInit();
    rtosObjectsInit();
}

 static void createApplicationTasks(void)
{
     xTaskCreate(inputTask,
                 "Input",
                 INPUT_TASK_STACK_SIZE,
                 NULL,
                 INPUT_TASK_PRIORITY,
                 NULL);

    xTaskCreate(gateControlTask,
                "GateCtrl",
                GATE_CONTROL_TASK_STACK_SIZE,
                NULL,
                GATE_CONTROL_TASK_PRIORITY,
                NULL);

    xTaskCreate(ledTask,
                "LED",
                LED_TASK_STACK_SIZE,
                NULL,
                LED_TASK_PRIORITY,
                NULL);

    xTaskCreate(safetyTask,
                "Safety",
                SAFETY_TASK_STACK_SIZE,
                NULL,
                SAFETY_TASK_PRIORITY,
                NULL);

 #if ENABLE_STATUS_TASK
     xTaskCreate(statusTask,
                 "Status",
                 STATUS_TASK_STACK_SIZE,
                 NULL,
                 STATUS_TASK_PRIORITY,
                 NULL);
 #endif
 }

int main(void)
{
    systemInit();

    createApplicationTasks();

    vTaskStartScheduler();

    while (1)
    {
    }
}