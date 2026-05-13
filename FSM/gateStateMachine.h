#ifndef GATE_STATE_MACHINE_H
#define GATE_STATE_MACHINE_H

#include <stdbool.h>
#include <stdint.h>

#include "types.h"
#include "events.h"

void gateStateMachineInit(void);

GateState gateStateMachineHandleEvent(GateEvent event);

GateState gateStateMachineGetState(void);
ControlMode gateStateMachineGetControlMode(void);
GateCommand gateStateMachineGetActiveCommand(void);
GateMovement gateStateMachineGetMovement(void);

void gateStateMachineFinishReversing(void);

bool gateStateMachineIsMoving(void);
bool gateStateMachineIsOpening(void);
bool gateStateMachineIsClosing(void);
bool gateStateMachineIsReversing(void);

#endif