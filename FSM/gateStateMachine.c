#include "gateStateMachine.h"
#include "appConfig.h"

static GateState currentGateState = GATE_IDLE_CLOSED;
static ControlMode currentControlMode = CONTROL_MODE_NONE;
static GateCommand activeCommand = COMMAND_NONE;

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
}

static void openGate(ControlMode mode)
{
	if(currentGateState != GATE_IDLE_OPEN) 
	{
		currentGateState = GATE_OPENING;
		currentControlMode = mode;
		activeCommand = COMMAND_OPEN;
	}
}

static void closeGate(ControlMode mode)
{
	if(currentGateState != GATE_IDLE_CLOSED) 
	{
		currentGateState = GATE_CLOSING;
		currentControlMode = mode;
		activeCommand = COMMAND_CLOSE;
	}
}

static bool isSamePanelConflict(void)
{
	if(securityOpenPressed && securityClosePressed) return true;
	
	if(driverOpenPressed && driverClosePressed) return true;
	
	return false;
}

static GateCommand getPriorityCommand(EventSource *source)
{
	if(securityOpenPressed && !securityClosePressed)
	{
		*source = SOURCE_SECURITY;
		return COMMAND_OPEN;
	}
	
		if(securityClosePressed && !securityOpenPressed)
	{
		*source = SOURCE_SECURITY;
		return COMMAND_CLOSE;
	}
	
		if(driverOpenPressed && !driverClosePressed)
	{
		*source = SOURCE_DRIVER;
		return COMMAND_OPEN;
	}
	
		if(driverClosePressed && !driverOpenPressed)
	{
		*source = SOURCE_DRIVER;
		return COMMAND_CLOSE;
	}
	
	*source = SOURCE_NONE;
	return COMMAND_NONE;

}


static uint32_t getPressedDuration(GateEvent event)
{
	if(lastPressedCommand == COMMAND_NONE) return 0;
	
	if(event.timestampMs < lastPressedTime) return 0;
	
	return (event.timestampMs - lastPressedTime);

}

static bool isReleaseOfActiveButton(GateEvent event)
{
	if(event.type == EVENT_DRIVER_OPEN_RELEASED && lastPressedCommand ==  COMMAND_OPEN && lastPressedSource == SOURCE_DRIVER) return true;
	
	if(event.type == EVENT_DRIVER_CLOSE_RELEASED && lastPressedCommand ==  COMMAND_CLOSE && lastPressedSource == SOURCE_DRIVER) return true;
	
	if(event.type == EVENT_SECURITY_OPEN_RELEASED && lastPressedCommand ==  COMMAND_OPEN && lastPressedSource == SOURCE_SECURITY) return true;
	
	if(event.type == EVENT_SECURITY_CLOSE_RELEASED && lastPressedCommand ==  COMMAND_CLOSE && lastPressedSource == SOURCE_SECURITY) return true;
	
	return false;
	
}

static void updateButtonMemory(GateEvent event)
{
	switch(event.type)
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
			driverOpenPressed = false;
			driverOpenPressedTime = event.timestampMs;
			lastPressedCommand = COMMAND_CLOSE;
			lastPressedSource = SOURCE_DRIVER;
			lastPressedTime = event.timestampMs;
			break;
		
		case EVENT_DRIVER_CLOSE_RELEASED:
      driverOpenPressed = true;
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
			securityOpenPressed = false;
			securityOpenPressedTime = event.timestampMs;
			lastPressedCommand = COMMAND_CLOSE;
			lastPressedSource = SOURCE_SECURITY;
			lastPressedTime = event.timestampMs;
			break;
		
		case EVENT_SECURITY_CLOSE_RELEASED:
      securityOpenPressed = true;
      break;

		default:
			break;
		
	}
  (void)driverOpenPressedTime;
  (void)driverClosePressedTime;
  (void)securityOpenPressedTime;
  (void)securityClosePressedTime;
}


