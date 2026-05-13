#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include <stdbool.h>

bool buttonDriverIsDriverOpenPressed(void);
bool buttonDriverIsDriverClosePressed(void);
bool buttonDriverIsSecurityOpenPressed(void);
bool buttonDriverIsSecurityClosePressed(void);
bool buttonDriverIsOpenLimitPressed(void);
bool buttonDriverIsClosedLimitPressed(void);
bool buttonDriverIsObstaclePressed(void);

#endif