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

int InterruptSystem::resolveInterrupt(  )
{	
	Interrupt currentInterrupt = interruptQueue.front( );
	int processNumber = currentInterrupt.processNumber;
	myLog.logProcess( "Interrupt: " + currentInterrupt.endLogMessage );
	interruptQueue.pop_front();
	numberOfInterrupts--;
	return processNumber;
}