#include <stdint.h>
#include <stdbool.h>

#include "TM4C123GH6PM.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define HANDLER_DELAY_MS 2000

SemaphoreHandle_t xCountingSemaphore;

volatile uint32_t TokensGenerated = 0;
volatile uint32_t TokensProcessed = 0;
volatile uint32_t TriggerTaskFinished = 0;

void vHandlerTask(void *pvParameters);
void vTriggerTask(void *pvParameters);
void TIMER0A_Handler(void);

int main(void)
{
    xCountingSemaphore = xSemaphoreCreateCounting(10, 0);

    NVIC_ClearPendingIRQ(TIMER0A_IRQn);
    NVIC_SetPriority(TIMER0A_IRQn, 7);
    NVIC_EnableIRQ(TIMER0A_IRQn);

    xTaskCreate(vHandlerTask, "Handler", 128, NULL, 2, NULL);
    xTaskCreate(vTriggerTask, "Trigger", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1)
    {
    }
}

void vHandlerTask(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);

        TokensProcessed++;

        vTaskDelay(pdMS_TO_TICKS(HANDLER_DELAY_MS));
    }
}

void vTriggerTask(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(1000));

    NVIC_SetPendingIRQ(TIMER0A_IRQn);

    vTaskDelay(pdMS_TO_TICKS(100));

    NVIC_SetPendingIRQ(TIMER0A_IRQn);
    NVIC_SetPendingIRQ(TIMER0A_IRQn);

    TriggerTaskFinished = 1;

    vTaskSuspend(NULL);
}

void TIMER0A_Handler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    NVIC_ClearPendingIRQ(TIMER0A_IRQn);

    TokensGenerated++;

    xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}