#include "gateStateMachine.h"
#include "appConfig.h"

static GateState currentGateState = GATE_IDLE_CLOSED;
static ControlMode currentControlMode = CONTROL_MODE_NONE;
static GateCommand activeCommand = COMMAND_NONE;
static EventSource activeCommandSource = SOURCE_NONE;

static bool driverOpenPressed = false;
static bool driverClosePressed = false;
static bool securityOpenPressed = false;
static bool securityClosePressed = false;

static uint32_t driverOpenPressedTime = 0;
static uint32_t driverClosePressedTime = 0;
static uint32_t securityOpenPressedTime = 0;
static uint32_t securityClosePressedTime = 0;

static GateCommand lastPressedCommand = COMMAND_NONE;
static EventSource lastPressedSource = SOURCE_NONE;
static uint32_t lastPressedTime = 0;

static void stopGate(void)
{
    currentGateState = GATE_STOPPED_MIDWAY;
    currentControlMode = CONTROL_MODE_NONE;
    activeCommand = COMMAND_STOP;
    activeCommandSource = SOURCE_NONE;
}

static void openGate(ControlMode mode, EventSource source)
{
    if (currentGateState != GATE_IDLE_OPEN)
    {
        currentGateState = GATE_OPENING;
        currentControlMode = mode;
        activeCommand = COMMAND_OPEN;
        activeCommandSource = source;
    }
}

static void closeGate(ControlMode mode, EventSource source)
{
    if (currentGateState != GATE_IDLE_CLOSED)
    {
        currentGateState = GATE_CLOSING;
        currentControlMode = mode;
        activeCommand = COMMAND_CLOSE;
        activeCommandSource = source;
    }
}

static bool isSamePanelConflict(void)
{
    if (securityOpenPressed && securityClosePressed)
    {
        return true;
    }

    if (driverOpenPressed && driverClosePressed)
    {
        return true;
    }

    return false;
}

static GateCommand getPriorityCommand(EventSource *source)
{
    if (securityOpenPressed && !securityClosePressed)
    {
        *source = SOURCE_SECURITY;
        return COMMAND_OPEN;
    }

    if (securityClosePressed && !securityOpenPressed)
    {
        *source = SOURCE_SECURITY;
        return COMMAND_CLOSE;
    }

    if (driverOpenPressed && !driverClosePressed)
    {
        *source = SOURCE_DRIVER;
        return COMMAND_OPEN;
    }

    if (driverClosePressed && !driverOpenPressed)
    {
        *source = SOURCE_DRIVER;
        return COMMAND_CLOSE;
    }

    *source = SOURCE_NONE;
    return COMMAND_NONE;
}

static uint32_t getPressedDuration(GateEvent event)
{
    if (lastPressedCommand == COMMAND_NONE)
    {
        return 0;
    }

    if (event.timestampMs < lastPressedTime)
    {
        return 0;
    }

    return event.timestampMs - lastPressedTime;
}

static bool isReleaseOfActiveButton(GateEvent event)
{
    if (event.type == EVENT_DRIVER_OPEN_RELEASED &&
        lastPressedCommand == COMMAND_OPEN &&
        lastPressedSource == SOURCE_DRIVER)
    {
        return true;
    }

    if (event.type == EVENT_DRIVER_CLOSE_RELEASED &&
        lastPressedCommand == COMMAND_CLOSE &&
        lastPressedSource == SOURCE_DRIVER)
    {
        return true;
    }

    if (event.type == EVENT_SECURITY_OPEN_RELEASED &&
        lastPressedCommand == COMMAND_OPEN &&
        lastPressedSource == SOURCE_SECURITY)
    {
        return true;
    }

    if (event.type == EVENT_SECURITY_CLOSE_RELEASED &&
        lastPressedCommand == COMMAND_CLOSE &&
        lastPressedSource == SOURCE_SECURITY)
    {
        return true;
    }

    return false;
}

static void updateButtonMemory(GateEvent event)
{
    switch (event.type)
    {
        case EVENT_DRIVER_OPEN_PRESSED:
            driverOpenPressed = true;
            driverOpenPressedTime = event.timestampMs;
            lastPressedCommand = COMMAND_OPEN;
            lastPressedSource = SOURCE_DRIVER;
            lastPressedTime = event.timestampMs;
            break;

        case EVENT_DRIVER_OPEN_RELEASED:
            driverOpenPressed = false;
            break;

        case EVENT_DRIVER_CLOSE_PRESSED:
            driverClosePressed = true;
            driverClosePressedTime = event.timestampMs;
            lastPressedCommand = COMMAND_CLOSE;
            lastPressedSource = SOURCE_DRIVER;
            lastPressedTime = event.timestampMs;
            break;

        case EVENT_DRIVER_CLOSE_RELEASED:
            driverClosePressed = false;
            break;

        case EVENT_SECURITY_OPEN_PRESSED:
            securityOpenPressed = true;
            securityOpenPressedTime = event.timestampMs;
            lastPressedCommand = COMMAND_OPEN;
            lastPressedSource = SOURCE_SECURITY;
            lastPressedTime = event.timestampMs;
            break;

        case EVENT_SECURITY_OPEN_RELEASED:
            securityOpenPressed = false;
            break;

        case EVENT_SECURITY_CLOSE_PRESSED:
            securityClosePressed = true;
            securityClosePressedTime = event.timestampMs;
            lastPressedCommand = COMMAND_CLOSE;
            lastPressedSource = SOURCE_SECURITY;
            lastPressedTime = event.timestampMs;
            break;

        case EVENT_SECURITY_CLOSE_RELEASED:
            securityClosePressed = false;
            break;

        default:
            break;
    }

    (void)driverOpenPressedTime;
    (void)driverClosePressedTime;
    (void)securityOpenPressedTime;
    (void)securityClosePressedTime;
}

static void evaluatePressedButtons(void)
{
    EventSource source = SOURCE_NONE;
    GateCommand command = COMMAND_NONE;

    if (currentControlMode == CONTROL_MODE_AUTO &&
        activeCommandSource == SOURCE_SECURITY &&
        !securityOpenPressed &&
        !securityClosePressed)
    {
        return;
    }

    if (isSamePanelConflict())
    {
        stopGate();
        return;
    }

    command = getPriorityCommand(&source);

    if (command == COMMAND_OPEN)
    {
        openGate(CONTROL_MODE_MANUAL, source);
    }
    else if (command == COMMAND_CLOSE)
    {
        closeGate(CONTROL_MODE_MANUAL, source);
    }
}

static void handleButtonRelease(GateEvent event)
{
    uint32_t duration = getPressedDuration(event);
    EventSource source = SOURCE_NONE;
    GateCommand remainingCommand = COMMAND_NONE;
    GateCommand releasedCommand = lastPressedCommand;
    EventSource releasedSource = lastPressedSource;

    if (!isReleaseOfActiveButton(event))
    {
        evaluatePressedButtons();
        return;
    }

    if (duration < AUTO_PRESS_THRESHOLD_MS)
    {
        currentControlMode = CONTROL_MODE_AUTO;
        activeCommand = releasedCommand;
        activeCommandSource = releasedSource;
        return;
    }

    remainingCommand = getPriorityCommand(&source);

    if (remainingCommand == COMMAND_OPEN)
    {
        openGate(CONTROL_MODE_MANUAL, source);
        lastPressedCommand = COMMAND_OPEN;
        lastPressedSource = source;
        lastPressedTime = event.timestampMs;
        return;
    }

    if (remainingCommand == COMMAND_CLOSE)
    {
        closeGate(CONTROL_MODE_MANUAL, source);
        lastPressedCommand = COMMAND_CLOSE;
        lastPressedSource = source;
        lastPressedTime = event.timestampMs;
        return;
    }

    stopGate();
    lastPressedCommand = COMMAND_NONE;
    lastPressedSource = SOURCE_NONE;
}

static void handleOpenLimit(void)
{
    if (currentGateState == GATE_OPENING || currentGateState == GATE_REVERSING)
    {
        currentGateState = GATE_IDLE_OPEN;
        currentControlMode = CONTROL_MODE_NONE;
        activeCommand = COMMAND_NONE;
        activeCommandSource = SOURCE_NONE;
        lastPressedCommand = COMMAND_NONE;
        lastPressedSource = SOURCE_NONE;
    }
}

static void handleClosedLimit(void)
{
    if (currentGateState == GATE_CLOSING)
    {
        currentGateState = GATE_IDLE_CLOSED;
        currentControlMode = CONTROL_MODE_NONE;
        activeCommand = COMMAND_NONE;
        activeCommandSource = SOURCE_NONE;
        lastPressedCommand = COMMAND_NONE;
        lastPressedSource = SOURCE_NONE;
    }
}

static void handleObstacle(void)
{
    if (currentGateState == GATE_CLOSING)
    {
        currentGateState = GATE_REVERSING;
        currentControlMode = CONTROL_MODE_AUTO;
        activeCommand = COMMAND_OPEN;
        activeCommandSource = SOURCE_SAFETY;
    }
}

void gateStateMachineInit(void)
{
    currentGateState = GATE_IDLE_CLOSED;
    currentControlMode = CONTROL_MODE_NONE;
    activeCommand = COMMAND_NONE;
    activeCommandSource = SOURCE_NONE;

    driverOpenPressed = false;
    driverClosePressed = false;
    securityOpenPressed = false;
    securityClosePressed = false;

    driverOpenPressedTime = 0;
    driverClosePressedTime = 0;
    securityOpenPressedTime = 0;
    securityClosePressedTime = 0;

    lastPressedCommand = COMMAND_NONE;
    lastPressedSource = SOURCE_NONE;
    lastPressedTime = 0;
}

GateState gateStateMachineHandleEvent(GateEvent event)
{
    updateButtonMemory(event);

    switch (event.type)
    {
        case EVENT_DRIVER_OPEN_PRESSED:
        case EVENT_DRIVER_CLOSE_PRESSED:
        case EVENT_SECURITY_OPEN_PRESSED:
        case EVENT_SECURITY_CLOSE_PRESSED:
            evaluatePressedButtons();
            break;

        case EVENT_DRIVER_OPEN_RELEASED:
        case EVENT_DRIVER_CLOSE_RELEASED:
        case EVENT_SECURITY_OPEN_RELEASED:
        case EVENT_SECURITY_CLOSE_RELEASED:
            handleButtonRelease(event);
            break;

        case EVENT_OPEN_LIMIT_PRESSED:
            handleOpenLimit();
            break;

        case EVENT_CLOSED_LIMIT_PRESSED:
            handleClosedLimit();
            break;

        case EVENT_OBSTACLE_DETECTED:
            handleObstacle();
            break;

        case EVENT_CONFLICT_DETECTED:
            stopGate();
            break;

        default:
            break;
    }

    return currentGateState;
}

GateState gateStateMachineGetState(void)
{
    return currentGateState;
}

ControlMode gateStateMachineGetControlMode(void)
{
    return currentControlMode;
}

GateCommand gateStateMachineGetActiveCommand(void)
{
    return activeCommand;
}

GateMovement gateStateMachineGetMovement(void)
{
    if (currentGateState == GATE_OPENING || currentGateState == GATE_REVERSING)
    {
        return GATE_MOTOR_OPEN;
    }

    if (currentGateState == GATE_CLOSING)
    {
        return GATE_MOTOR_CLOSE;
    }

    return GATE_MOTOR_STOP;
}

void gateStateMachineFinishReversing(void)
{
    if (currentGateState == GATE_REVERSING)
    {
        currentGateState = GATE_STOPPED_MIDWAY;
        currentControlMode = CONTROL_MODE_NONE;
        activeCommand = COMMAND_STOP;
        activeCommandSource = SOURCE_NONE;
        lastPressedCommand = COMMAND_NONE;
        lastPressedSource = SOURCE_NONE;
    }
}

bool gateStateMachineIsMoving(void)
{
    return currentGateState == GATE_OPENING ||
           currentGateState == GATE_CLOSING ||
           currentGateState == GATE_REVERSING;
}

bool gateStateMachineIsOpening(void)
{
    return currentGateState == GATE_OPENING;
}

bool gateStateMachineIsClosing(void)
{
    return currentGateState == GATE_CLOSING;
}

bool gateStateMachineIsReversing(void)
{
    return currentGateState == GATE_REVERSING;
}