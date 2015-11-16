#ifndef STRUCTS_H
#define STRUCTS_H

struct Process
{
	char component;
	string operation;
	int numberOfCycles;
};

struct PcbThread
{
	string startLogMessage;
	string endLogMessage;
	int timePerCyle;
	int numCyclesRemaining;
};

enum InterruptType
{
	IO = 0,
	QUANTUM = 1
};

struct Interrupt
{
	InterruptType interruptType;
	int priority; // Redundant use of memory, but used to save processing later on
	int pcbID;
};
#endif // STRUCTS_H