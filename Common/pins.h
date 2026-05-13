#ifndef PINS_H
#define PINS_H

#include <stdint.h>
#include <stdbool.h>

#include "hw_memmap.h"
#include "gpio.h"

#define GREEN_LED_PORT          GPIO_PORTF_BASE
#define GREEN_LED_PIN           GPIO_PIN_3

#define RED_LED_PORT            GPIO_PORTF_BASE
#define RED_LED_PIN             GPIO_PIN_1

#define DRIVER_OPEN_PORT        GPIO_PORTF_BASE
#define DRIVER_OPEN_PIN         GPIO_PIN_0

#define DRIVER_CLOSE_PORT       GPIO_PORTF_BASE
#define DRIVER_CLOSE_PIN        GPIO_PIN_4

#define SECURITY_OPEN_PORT      GPIO_PORTE_BASE
#define SECURITY_OPEN_PIN       GPIO_PIN_0

#define SECURITY_CLOSE_PORT     GPIO_PORTE_BASE
#define SECURITY_CLOSE_PIN      GPIO_PIN_1

#define OPEN_LIMIT_PORT         GPIO_PORTB_BASE
#define OPEN_LIMIT_PIN          GPIO_PIN_0

#define CLOSED_LIMIT_PORT       GPIO_PORTB_BASE
#define CLOSED_LIMIT_PIN        GPIO_PIN_1

#define OBSTACLE_PORT           GPIO_PORTD_BASE
#define OBSTACLE_PIN            GPIO_PIN_0

#endif