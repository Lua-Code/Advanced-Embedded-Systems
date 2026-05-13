#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    GATE_IDLE_CLOSED = 0,
    GATE_IDLE_OPEN,
    GATE_OPENING,
    GATE_CLOSING,
    GATE_STOPPED_MIDWAY,
    GATE_REVERSING
} GateState;

typedef enum
{
    GATE_MOTOR_STOP = 0,
    GATE_MOTOR_OPEN,
    GATE_MOTOR_CLOSE
} GateMovement;

typedef enum
{
    COMMAND_NONE = 0,
    COMMAND_OPEN,
    COMMAND_CLOSE,
    COMMAND_STOP
} GateCommand;

typedef enum
{
    CONTROL_MODE_NONE = 0,
    CONTROL_MODE_MANUAL,
    CONTROL_MODE_AUTO
} ControlMode;

typedef enum
{
    SOURCE_NONE = 0,
    SOURCE_DRIVER,
    SOURCE_SECURITY,
    SOURCE_LIMIT,
    SOURCE_SAFETY
} EventSource;

typedef enum
{
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED
} ButtonState;

#endif