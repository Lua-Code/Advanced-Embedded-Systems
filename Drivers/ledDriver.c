#include "ledDriver.h"
#include "pins.h"

#include "gpio.h"

void ledDriverGreenOn(void)
{
    GPIOPinWrite(GREEN_LED_PORT, GREEN_LED_PIN, GREEN_LED_PIN);
}

void ledDriverGreenOff(void)
{
    GPIOPinWrite(GREEN_LED_PORT, GREEN_LED_PIN, 0);
}

void ledDriverRedOn(void)
{
    GPIOPinWrite(RED_LED_PORT, RED_LED_PIN, RED_LED_PIN);
}

void ledDriverRedOff(void)
{
    GPIOPinWrite(RED_LED_PORT, RED_LED_PIN, 0);
}

void ledDriverAllOff(void)
{
    GPIOPinWrite(GREEN_LED_PORT, GREEN_LED_PIN, 0);
    GPIOPinWrite(RED_LED_PORT, RED_LED_PIN, 0);
}
