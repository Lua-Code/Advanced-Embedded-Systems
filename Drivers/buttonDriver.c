#include "buttonDriver.h"
#include "pins.h"

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"

//return true if button is pressed and pull down
static bool buttonDriverReadActiveHigh(uint32_t port, uint8_t pin)
{
    return (GPIOPinRead(port, pin) & pin) != 0;
}
//return true if button is pressed and pull up
static bool buttonDriverReadActiveLow(uint32_t port, uint8_t pin)
{
    return (GPIOPinRead(port, pin) & pin) == 0;
}

bool buttonDriverIsDriverOpenPressed(void)
{
    return buttonDriverReadActiveLow(DRIVER_OPEN_PORT, DRIVER_OPEN_PIN);
}

bool buttonDriverIsDriverClosePressed(void)
{
    return buttonDriverReadActiveLow(DRIVER_CLOSE_PORT, DRIVER_CLOSE_PIN);
}

bool buttonDriverIsSecurityOpenPressed(void)
{
    return buttonDriverReadActiveHigh(SECURITY_OPEN_PORT, SECURITY_OPEN_PIN);
}

bool buttonDriverIsSecurityClosePressed(void)
{
    return buttonDriverReadActiveHigh(SECURITY_CLOSE_PORT, SECURITY_CLOSE_PIN);
}

bool buttonDriverIsOpenLimitPressed(void)
{
    return buttonDriverReadActiveHigh(OPEN_LIMIT_PORT, OPEN_LIMIT_PIN);
}

bool buttonDriverIsClosedLimitPressed(void)
{
    return buttonDriverReadActiveHigh(CLOSED_LIMIT_PORT, CLOSED_LIMIT_PIN);
}

bool buttonDriverIsObstaclePressed(void)
{
    return buttonDriverReadActiveHigh(OBSTACLE_PORT, OBSTACLE_PIN);
}
