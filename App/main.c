#include "TM4C123GH6PM.h"

#include <stdint.h>
#include <stdbool.h>

#include "sysctl.h"

#include "gpioDriver.h"
#include "ledDriver.h"
#include "buttonDriver.h"

static void delayMs(uint32_t ms)
{
    SysCtlDelay((SysCtlClockGet() / 3000) * ms);
}

static void blinkGreen(uint32_t count)
{
    uint32_t i;

    for (i = 0; i < count; i++)
    {
        ledDriverGreenOn();
        ledDriverRedOff();
        delayMs(150);
        ledDriverAllOff();
        delayMs(150);
    }
}

static void blinkRed(uint32_t count)
{
    uint32_t i;

    for (i = 0; i < count; i++)
    {
        ledDriverRedOn();
        ledDriverGreenOff();
        delayMs(150);
        ledDriverAllOff();
        delayMs(150);
    }
}

static void blinkBoth(uint32_t count)
{
    uint32_t i;

    for (i = 0; i < count; i++)
    {
        ledDriverGreenOn();
        ledDriverRedOn();
        delayMs(150);
        ledDriverAllOff();
        delayMs(150);
    }
}

int main(void)
{
 //   SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    gpioDriverInit();
    ledDriverAllOff();

    while (1)
    {
        if (buttonDriverIsDriverOpenPressed())
        {
            ledDriverGreenOn();
            ledDriverRedOff();
        }
        else if (buttonDriverIsDriverClosePressed())
        {
            ledDriverGreenOff();
            ledDriverRedOn();
        }
        else if (buttonDriverIsSecurityOpenPressed())
        {
            blinkGreen(2);
        }
        else if (buttonDriverIsSecurityClosePressed())
        {
            blinkRed(2);
        }
        else if (buttonDriverIsOpenLimitPressed())
        {
            blinkBoth(1);
        }
        else if (buttonDriverIsClosedLimitPressed())
        {
            blinkBoth(2);
        }
        else if (buttonDriverIsObstaclePressed())
        {
            blinkBoth(4);
        }
        else
        {
            ledDriverAllOff();
        }

        delayMs(20);
    }
}