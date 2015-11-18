#ifndef INTERRUPT_SYSTEM_H
#define INTERRUPT_SYSTEM_H

#include <deque>
#include <string>
#include "Structs.h"

using namespace std;

class InterruptSystem
{
public:
	Interrupt getCurrentInterrupt( );
	void addNewInterruptFront( InterruptType interruptType, int processNumber, string endLogMessage );
	void addNewInterruptBack( InterruptType interruptType, int processNumber, string endLogMessage );
	bool resolveInterrupt( int priority );

	int numberOfInterrupts;
private:
	// dequeue allows me to push IO interrupts to the back, while simultaneously pushing
	// quantum interrupts to the front; uses a bit more memory, but will be made up by
	// the fact that we could have a very large number of interrupts. Added benefits:
	// cost of reallocation decreased, and don't have to allocate everything in a
	// contiguous block
	deque< Interrupt > interruptQueue;
};

#endif // INTERRUPT_SYSTEM_H