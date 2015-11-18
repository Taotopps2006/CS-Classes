#include "InterruptSystem.h"

Interrupt InterruptSystem::getCurrentInterrupt( )
{
	return interruptQueue.front();
}

void InterruptSystem::addNewInterruptFront( InterruptType interruptType, int processNumber, string endLogMessage )
{
	Interrupt currentInterrupt;
	currentInterrupt.interruptType = interruptType;
	currentInterrupt.processNumber = processNumber;
	currentInterrupt.endLogMessage = endLogMessage;
	interruptQueue.push_front(currentInterrupt);
	numberOfInterrupts++;
}

void InterruptSystem::addNewInterruptBack( InterruptType interruptType, int processNumber, string endLogMessage )
{
	Interrupt currentInterrupt;
	currentInterrupt.interruptType = interruptType;
	currentInterrupt.processNumber = processNumber;
	currentInterrupt.endLogMessage = endLogMessage;
	interruptQueue.push_back(currentInterrupt);
	numberOfInterrupts++;
}

bool InterruptSystem::removeInterrupt( int priority )
{
	if(numberOfInterrupts == 0)
	{
		return false;
	}
	interruptQueue.pop_front();
	numberOfInterrupts--;
	return true;
}