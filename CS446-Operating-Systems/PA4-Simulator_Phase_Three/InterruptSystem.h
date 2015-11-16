#ifndef INTERRUPT_SYSTEM_H
#define INTERRUPT_SYSTEM_H

#include <map>
#include "Structs.h"

using namespace std;

class InterruptSystem
{
public:
	Interrupt getCurrentInterrupt();
	void addNewInterrupt(InterruptType interruptType, int pcbID);
	bool removeInterrupt(int priority);
private:
	map<int, Interrupt> interruptQueue; // Simulates a priority queue with random access ability
};

#endif // INTERRUPT_SYSTEM_H