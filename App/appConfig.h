#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#define SYSTEM_CLOCK_HZ                 16000000UL

#define INPUT_TASK_PRIORITY             3
#define GATE_CONTROL_TASK_PRIORITY      2
#define LED_TASK_PRIORITY               2
#define SAFETY_TASK_PRIORITY            4
#define STATUS_TASK_PRIORITY            1

#define INPUT_TASK_STACK_SIZE           128
#define GATE_CONTROL_TASK_STACK_SIZE    128
#define LED_TASK_STACK_SIZE             128
#define SAFETY_TASK_STACK_SIZE          128
#define STATUS_TASK_STACK_SIZE          128

#define GATE_EVENT_QUEUE_LENGTH         16

#define BUTTON_DEBOUNCE_MS              30
#define INPUT_TASK_PERIOD_MS            20
#define LED_TASK_PERIOD_MS              50
#define STATUS_TASK_PERIOD_MS           200

#define AUTO_PRESS_THRESHOLD_MS         300
#define OBSTACLE_REVERSE_TIME_MS        500

#endif